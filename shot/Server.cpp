#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <unistd.h>
#include <memory.h>
#include <memory>
#include <cstring>
#include <iterator>
#include <iostream>
#include "Server.h"
#include "Handler.h"
#include "http.h"
#include "system_handlers.h"


using std::ostringstream;


namespace shot {


Server::Server()
    : errorCode(0)
    , factory400(factory<E400Handler>)
    , factory403(factory<E403Handler>)
    , factory404(factory<E404Handler>)
    , factory500(factory<E500Handler>) {
}


Server::~Server() {
  freeHandlers();
  close(listenSocket);
  close(epfd);
}


Server& Server::instance() {
  static Server server;
  return server;
}


void Server::run() {
  // reset listen address
  sockaddr_in addr;
  memset(&addr, 0, sizeof(struct sockaddr_in));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(Config::instance().port);
  addr.sin_addr.s_addr = INADDR_ANY;

  // create listenSocket
  listenSocket = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);

  if (listenSocket == -1) {
    errorCode = -1;
    Logger::instance().error(errorCode, "listenSocket not created");
    return;
  }

  // set socket reuse option
  int optValue = 1;
  socklen_t optLength = sizeof(optValue);
  setsockopt(listenSocket, SOL_SOCKET, SO_REUSEADDR, (void*)&optValue, optLength);

  // bind listenSocket
  if (bind(listenSocket, (struct sockaddr*)&addr, sizeof(struct sockaddr)) == -1) {
    errorCode = -1;
    Logger::instance().error(errorCode, "listenSocket bind error");
    return;
  }

  // listen listenSocket
  if (listen(listenSocket, 1) == -1) {
    errorCode = -1;
    Logger::instance().error(errorCode, "listen listenSocket error");
    return;
  }

  // create epoll
  epfd = epoll_create1(0);

  if (epfd == -1) {
    errorCode = -1;
    Logger::instance().error(errorCode, "epoll create error");
    return;
  }

  auto EPOLL_BAD_EVENTS = EPOLLERR | EPOLLHUP | EPOLLRDHUP;
  auto EPOLL_CLIENT_EVENTS = EPOLLIN | EPOLLET | EPOLL_BAD_EVENTS;

  if (!epollAdd(listenSocket, EPOLLIN | EPOLLET)) {
    Logger::instance().error(errorCode, "add listenSocket to epoll error");
  }

  // for 
  int clientSocket;
  sockaddr clientAddr;
  socklen_t clientAddrSize;
  epoll_event events[Config::instance().maxEvents];

  int count;
  int n;

  // run event loop
  for (;;) {
    // wait epoll events (listenSocket, clientSocket)
    count = epoll_wait(epfd, events, Config::instance().maxEvents, -1);

    if (count == -1) {
      errorCode = -1;
      Logger::instance().error(errorCode, "epoll_wait error");
      return;
    }

    // process events
    for (n = 0; n < count; ++n) {

      if ((events[n].events & EPOLL_BAD_EVENTS) ||
          (!(events[n].events & EPOLLIN))) {
        closeHandler(events[n].data.fd);
        continue;
      }

      // process listenSocket event
      if (events[n].data.fd == listenSocket) {
        // request, handler
        // we have incoming connections, process them all
        for (;;) {
          // accept clientCocket
          clientSocket = accept(listenSocket, &clientAddr, &clientAddrSize);

          if (clientSocket == -1) {
            if ((errno == EAGAIN) ||
                (errno == EWOULDBLOCK)) {
              // we have processed all incoming connections
              break;
            } else {
              Logger::instance().error(-1, "clientSocket accept error");
              break;
            }
          }

          // set clientSocket non blocking
          if (fcntl(clientSocket, F_SETFL, fcntl(clientSocket, F_GETFD, 0) | O_NONBLOCK) == -1) {
            Logger::instance().error(-1, "clientSocket setnonblocking error");
            break;
          }

          // add clientSocket to epoll
          if (!epollAdd(clientSocket, EPOLL_CLIENT_EVENTS)) {
            Logger::instance().error(-1, "clientSocket add to epoll error");
            break;
          }
        }
      } else { // process data from clientSocket
        clientSocket = events[n].data.fd;
        // TODO: optimize - eliminate ostringstream, use member char buffer[size]
        std::ostringstream buffer;
        /* bool systemHandler = false; */
        
        // read socket
        readSocket(clientSocket, buffer);
        string source = buffer.str();
        size_t sourceLength = source.length();

        // search handler by socket
        Handler* handler = getHandler(clientSocket);

        if (handler) { // read body
          parser.parseBody(*(handler->request), source, 0, sourceLength);
        } else { // read head
          std::unique_ptr<Request> request(new Request());

          parser.parse(*request, source, sourceLength);

          // create and init handler
          handler = createHandler(clientSocket, *request);
          handler->socket = clientSocket;
          handler->request = request.release();
        }

        // check request done
        if (handler->request->isDone) {
          // call handler method
          switch (handler->request->method) {
            case C_GET:
              if (handler->request->isXhr()) handler->xget();
              else handler->get();
              break;
            case C_POST:
              if (handler->request->isXhr()) handler->xpost();
              else handler->post();
              break;
            default:
              handler->response.status = HTTP_403;
              break;
          }

          // check handler status and replace by system handler
          if (/*handler->response.content.rdbuf()->in_avail() == 0 and*/
              handler->response.status >= 400 and
              handler->response.status < 600) {

            // unregister user handler
            Request* request = handler->request;
            handler->request = nullptr;
            int responseStatus = handler->response.status;
            string errorMessage = handler->response.error;

            handler = nullptr;

            freeHandler(clientSocket);

            switch (responseStatus) {
              case HTTP_400:
                handler = factory400(request);
                break;
              case HTTP_403:
                handler = factory403(request);
                break;
              case HTTP_404:
                handler = factory404(request);
                break;
              case HTTP_500:
                handler = factory500(request);
                break;
              default:
                Logger::instance().write("unimplemented http status: " + std::to_string(responseStatus));
                closeHandler(clientSocket);
                break;
            }

            if (handler) {
              // init system handler
              storeHandler(clientSocket, handler);
              handler->request = request;
              handler->socket = clientSocket;
              handler->response.error = errorMessage;
              // run system handler
              if (handler->request->isXhr()) {
                handler->xget();
              } else {
                handler->get();
              }
            }
          }

          std::cout << "response.content.eof:  " << handler->response.content.eof() << std::endl;

          // flush handler, only with content
          if (handler and handler->response.flush) {
            flushHandler(handler);
          }
        }
      }
    }
  }
}


void Server::stop() {
}


int Server::getErrorCode() {
  return errorCode;
}


void Server::setFactory(const string label, type_factory factory) {
  factories.insert({label, factory});
}


void Server::setFactories(type_factories& hs) {
  for (auto it = hs.begin(); it != hs.end(); ++it) {
    factories.insert({it->first, it->second});
  }
}


void Server::setFactory400(type_factory factory) {
  factory400 = factory;
}


void Server::setFactory403(type_factory factory) {
  factory403 = factory;
}


void Server::setFactory404(type_factory factory) {
  factory404 = factory;
}


void Server::setFactory500(type_factory factory) {
  factory500 = factory;
}


void Server::loop() {
}


Handler* Server::getHandler(int sock) {
  auto it = handlers.find(sock);
  return it == handlers.end() ? nullptr : it->second;
}


Handler* Server::createHandler(int sock, Request& request) {
  type_factory factory = nullptr;

  // check parse request
  if (request.isBad or request.slugs.empty() or request.method == C_NONE) {
    factory = factory403;
    request.isDone = true;
  } else {
    // find factory
    auto it = factories.find(request.slugs[0]);
    if (it == factories.end()) {
      factory = factory404;
      request.isDone = true;
    } else {
      factory = it->second;
    }
  }


  // create handler and store
  Handler* handler = factory(&request);
  if (handler) {
    storeHandler(sock, handler);
  }

  return handler;
}


void Server::storeHandler(int sock, Handler* handler) {
  handlers.insert({sock, handler});
}


void Server::closeHandler(int sock) {
  freeHandler(sock);
  close(sock);
}


void Server::freeHandler(int sock) {
  auto it = handlers.find(sock);
  if (it != handlers.end()) {
    delete it->second;
    handlers.erase(it);
  }
}


void Server::freeHandlers() {
  for (auto it = handlers.begin(); it != handlers.end(); ++it) {
    delete it->second;
  }
}


void Server::flushHandler(Handler* handler) {
  // check request header Connection: keep-alive
  // write response header Connection: keep-alive
  bool isKeepAlive = handler->request->isKeepAlive();
  if (isKeepAlive) {
    handler->response.setHeader(K_CONNECTION, V_KEEP_ALIVE);
  }

  string message = handler->response.toString();
  writeSocket(handler->socket, message.c_str(), message.length());

  // TODO: before close sock ensure sock state to EOF
  // TODO: check request header Connection: keep-alive
  if (!isKeepAlive) {
    closeHandler(handler->socket);
  } else {
    freeHandler(handler->socket);
  }
}


// TODO: rewrite this without loop and to char buf[]
void Server::readSocket(int sock, std::ostream& buffer) {
  int count; // count read from socket
  char buf[Config::instance().chunkSize]; // buffer for socket

  for (int i = 0;;++i) {
    count = read(sock, (void*)buf, Config::instance().chunkSize);

    if (count <= 0) {
      if (count == -1 and errno != EAGAIN) {
        Logger::instance().error(count, "readSocket error");
      }
      break;
    }

    // debug TODO: remove for if this not happen, and remove ostream, just
    // char*
    if (i >= 1) Logger::instance().write("readSocket i >= 1");

    /* std::cout.write(buf, count); */
    /* std::cout << std::endl; */

    buffer.write(buf, count);
  }

}

void Server::writeSocket(int sock, const char* buf, int size) {
  int total = 0;
  int count;
  for (;;) {
    count = write(sock, buf, size);

    if (count <= 0) {
      if (count < 0) {
        Logger::instance().error(count, "writeSocket error");
        return;
      } else {
        if (count != total) {
          Logger::instance().error(count, "writeSocket error count not equal total");
        }
      }
      break;
    }

    total += count;

    if (total == size) {
      break;
    }
  }
}


bool Server::epollAdd(int sock, uint32_t events) {
  static epoll_event ev;
  ev.events = events;
  ev.data.fd = sock;

  if (epoll_ctl(epfd, EPOLL_CTL_ADD, sock, &ev) == -1) {
    errorCode = -1;
    return false;
  }
  return true;
}


bool Server::epollDel(int sock) {
  if (epoll_ctl(epfd, EPOLL_CTL_DEL, sock, nullptr) == -1) {
    errorCode = -1;
    return false;
  }
  return true;
}


} /* namespace shot */
