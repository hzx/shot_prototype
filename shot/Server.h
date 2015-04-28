#ifndef SHOT_SERVER_H
#define SHOT_SERVER_H

#include <string>
#include <unordered_map>
#include <memory>
#include "Logger.h"
#include "Options.h"
#include "Request.h"
#include "Parser.h"
#include "Handler.h"


using std::string;
using std::unordered_map;


namespace shot {


typedef Handler* (*type_factory)(Request* request);
typedef unordered_map<string, type_factory> type_factories;


template<class T>
Handler* factory(Request* /*request*/) {
  return new T();
}


class Server {
public:
  Server();
  ~Server();
  static Server& instance();
  void run();
  void stop();
  int getErrorCode();
  void setFactory(const string label, type_factory factory);
  void setFactories(unordered_map<string, type_factory>& hs);

  void setFactory400(type_factory factory);
  void setFactory403(type_factory factory);
  void setFactory404(type_factory factory);
  void setFactory406(type_factory factory);
  void setFactory500(type_factory factory);

  void flushHandler(Handler* handler);


private:
  Parser parser;

  int errorCode;
  int listenSocket;
  int epfd;

  /* unordered_map<string, type_factory> factories; */
  type_factories factories;
  type_factory factory400;
  type_factory factory403;
  type_factory factory404;
  type_factory factory406;
  type_factory factory500;

  unordered_map<string, string> extHeaders;
  unordered_map<int, Handler*> handlers;

  void loop();
  Handler* getHandler(int sock);
  Handler* createHandler(int sock, Request& request);
  void storeHandler(int sock, Handler* handler);
  void closeHandler(int sock);
  void freeHandler(int sock);
  void freeHandlers();
  void readSocket(int sock, std::ostream& buffer);
  void writeSocket(int sock, const char* buf, int size);
  void processHead(int sock);
  void processBody(Handler* handler);

  bool epollAdd(int sock, uint32_t events);
  bool epollDel(int sock);

  // deprecated
  void processClient(Handler* handler);
};


} /* namespace shot */

#endif /* end of include guard: SHOT_SERVER_H */
