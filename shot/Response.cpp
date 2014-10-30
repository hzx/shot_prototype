#include <sstream>
#include "http.h"
#include "utils.h"
#include "Config.h"
#include "Response.h"


using std::ostringstream;


namespace shot {


Response::Response()
    : status(HTTP_200)
    , flush(true) {
}


string Response::toString() {
  ostringstream buf;

  // add status
  switch (status) {
  case HTTP_200:
    buf << HTTP_STATUS_200;
    break;
  case HTTP_301:
    buf << HTTP_STATUS_301;
    break;
  case HTTP_302:
    buf << HTTP_STATUS_302;
    break;
  case HTTP_400:
    buf << HTTP_STATUS_400;
    break;
  case HTTP_403:
    buf << HTTP_STATUS_403;
    break;
  case HTTP_404:
    buf << HTTP_STATUS_404;
    break;
  case HTTP_405:
    buf << HTTP_STATUS_405;
    break;
  case HTTP_406:
    buf << HTTP_STATUS_406;
    break;
  case HTTP_500:
    buf << HTTP_STATUS_500;
  }

  // calculate content length - content total length
  string contentString = content.str();
  int contentLength = contentString.length();
  /* for (auto it = content.begin(); it != content.end(); ++it) { */
  /*   contentLength += it->length(); */
  /* } */
  // add Content-length header
  buf << K_CONTENT_LENGTH_COLON << std::to_string(contentLength) << '\n';

  // add headers
  for (auto it = headers.begin(); it != headers.end(); ++it) {
    buf << it->first << ':' << it->second << '\n';
  }

  // add cookie
  if (!cookie.empty()) {
    for (auto it = cookie.begin(); it != cookie.end(); ++it) {
      buf << K_SET_COOKIE << ':' << it->first << '=' << it->second << "\n";
    }
  }

  // insert blank line
  buf << '\n';

  // add body content
  buf << contentString;
  /* for (auto it = content.begin(); it != content.end(); ++it) { */
  /*   buf << *it; */
  /* } */

  return buf.str();
}


void Response::setHeader(const char* name, const char* value) {
  headers.insert({name, value});
}


void Response::setTextHeader() {
  headers.insert({K_CONTENT_TYPE, V_TEXT_UTF8});
}


void Response::setHtmlHeader() {
  headers.insert({K_CONTENT_TYPE, V_HTML_UTF8});
}


void Response::setXmlHeader() {
  headers.insert({K_CONTENT_TYPE, V_XML_UTF8});
}


void Response::setJsonHeader() {
  headers.insert({K_CONTENT_TYPE, V_JSON_UTF8});
}


void Response::setStreamHeader() {
  headers.insert({K_CONTENT_TYPE, V_OCTET_STREAM});
}


void Response::setCookie(const char* name, const char* value, int expiresDays) {
  ostringstream buf;

  buf << value;

  if (expiresDays != 0) {
    buf << ";Max-Age=" << expiresDays * 86400;
  }

  buf << ";Path=/;HttpOnly";

  cookie.insert({name, buf.str()});
}


void Response::setSecureCookie(const char* name, const char* value, int expiresDays) {
  string enc = encodeCookie(Config::instance().cookieSecret, name, value, std::time(nullptr));
  setCookie(name, enc.data(), expiresDays);
}


void Response::clearCookie(const char* name) {
  cookie.insert({name, ";Max-Age=0;Path=/;HttpOnly"});
}


void Response::setStatus(int statuz) {
  status = statuz;
}


} /* namespace shot */

