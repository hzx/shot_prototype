#include <sstream>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <ctime>
#include "Handler.h"
#include "http.h"
#include "utils.h"
#include "Server.h"


using std::string;


namespace shot {


Handler::Handler()
    : request(nullptr)
    , state(HANDLER_STATE_NONE) {
}


Handler::~Handler() {
  if (request != nullptr) {
    delete request;
    request = nullptr;
  }
}


void Handler::get() {
  response.status = HTTP_406;
  response.setTextHeader();
  write("Ошибка 406");
}


void Handler::xget() {
  response.status = HTTP_406;
  response.setJsonHeader();
  write("{\"message\":\"Ошибка 406\"}");
}


void Handler::post() {
  response.status = HTTP_406;
  response.setTextHeader();
  write("Ошибка 406");
}


void Handler::xpost() {
  response.status = HTTP_406;
  response.setJsonHeader();
  write("{\"message\":\"Ошибка 406\"}");
}


string Handler::getXsrf() {
  // get from request or create
  if (xsrf.length() == 0) {
    auto cit = request->cookie.find(XSRF);
    if (cit != request->cookie.end()) {
      xsrf = cit->second;
    } else {
      xsrf = createXsrf();
    }
  }
  return xsrf;
}


void Handler::setXsrf() {
  auto cit = request->cookie.find(XSRF);
  if (cit != request->cookie.end()) {
    return;
  }

  string xsrf = getXsrf();
  response.setCookie(XSRF, xsrf.data(), 0);
}

  
void Handler::setError(int status, string message) {
  response.status = status;
  response.error = message;
}


void Handler::redirectPermanently(string location) {
  response.status = HTTP_301;
  response.setHeader(K_LOCATION, location.data());
  if (request->isXhr()) {
    response.setJsonHeader();
    write("{\"message\":\"Страница перемещена по адресу " + location +"\"}");
  } else {
    response.setHtmlHeader();
    write("<p>Страница перемещена по адресу <a href=\"" + location + "\">" + location + "</a></p>");
  }
}


void Handler::redirectTemporarily(string location) {
  response.status = HTTP_302;
  response.setHeader(K_LOCATION, location.data());
  if (request->isXhr()) {
    response.setJsonHeader();
    write("{\"message\":\"Страница перемещена по адресу " + location +"\"}");
  } else {
    response.setHtmlHeader();
    write("<p>Страница перемещена по адресу <a href=\"" + location + "\">" + location + "</a></p>");
  }
}


void Handler::write(const char* txt) {
  response.content.push_back(txt);
}


void Handler::write(std::string txt) {
  response.content.push_back(txt);
}


void Handler::flush() {
  Server::instance().flushHandler(this);
}


} /* namespace shot */
