/* #include <cstring> */
#include <ctime>
#include <iostream>
#include <cstring>
#include <cstdio>
#include "Request.h"
#include "Config.h"
#include "http.h"
#include "utils.h"
#include "Logger.h"


using std::string;


namespace shot {


File::File()
    : isCreated(false) {}


Request::Request()
    : isBad(false)
    , isDone(false)
    , method(C_NONE)
    , contentType(C_FORM_NONE)
    , contentLength(0)
    , contentProgress(0) {
}


Request::~Request() {
  removeFiles();
}


bool Request::isKeepAlive() {
  auto iter = headers.find(KL_CONNECTION);
  if (iter == headers.end()) return false;

  toLower(iter->second);
  /* return iter->second.compare(V_KEEP_ALIVE) == 0; */
  return std::memcmp(iter->second.c_str(), V_KEEP_ALIVE,
      std::strlen(V_KEEP_ALIVE)) == 0;
}


bool Request::isXhr() {
  auto iter = headers.find(KL_XHR);
  return iter != headers.end() and
    std::memcmp(iter->second.c_str(), V_XHR, std::strlen(V_XHR)) == 0;
}


bool Request::checkXsrf() {
  auto cit = cookie.find(XSRF);
  auto hit = headers.find(KL_XSRF);

  // xsrf must exists in cookie and headers
  if (cit == cookie.end() or hit == headers.end()) {
    Logger::instance().write("xsrf cookie or header not found");
    if (cit == cookie.end()) {
      Logger::instance().write("xsrf cookie not found");
      for (auto it = cookie.begin(); it != cookie.end(); ++it) {
        Logger::instance().write(it->first + '=' + it->second);
      }
    }
    if (hit == headers.end()) {
      Logger::instance().write("xsrf header not found");
    }
    return false;
  }

  // hosts must be equal, skip for empty config.host
  if (!Config::instance().host.empty() and
      Config::instance().host != host) {
    Logger::instance().write("xsrf host not equal"); 
    return false;
  }

  string cookieXsrf = decodeXsrf(cit->second);
  string headerXsrf = decodeXsrf(hit->second);

  if (cookieXsrf.length() == 0 or headerXsrf.length() == 0 or 
      cookieXsrf != headerXsrf) {
    Logger::instance().write("xsrf cookie and xsrf header not equal");
    return false;
  }

  return true;
}


string Request::getHeader(const char* name) {
  auto it = headers.find(name);
  return it == headers.end() ? "" : it->second;
}


string Request::getCookie(const char* name) {
  auto it = cookie.find(name);
  return it == cookie.end() ? "" : it->second;
}


string Request::getParam(const char* name) {
  auto it = params.find(name);
  return it == params.end() ? "" : it->second;
}


string Request::getSecureCookie(const char* name) {
  string value = getCookie(name);
  return value.empty() ? "" : decodeCookie(Config::instance().cookieSecret,
      name, value, 30, std::time(nullptr));
}


void Request::removeFiles() {
  for (auto i = files.begin(); i != files.end(); ++i) {
    std::remove(i->path.c_str());
  }
}


} /* namespace shot */
