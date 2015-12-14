#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <sstream>


using std::string;
using std::vector;
using std::unordered_map;
using std::ostringstream;


namespace shot {


class Response {
public:
  Response();
  void toString(string& headers, string& content);
  string toString();
  void setHeader(const char* name, const char* value);
  void setTextHeader();
  void setHtmlHeader();
  void setXmlHeader();
  void setJsonHeader();
  void setStreamHeader();
  void setCookie(const char* name, const char* value, int expiresDays = 30);
  void setSecureCookie(const char* name, const char* value, int expiresDays = 30);
  void clearCookie(const char* name);
  void setStatus(int status);

  int status;
  unordered_map<string, string> headers;
  unordered_map<string, string> cookie;
  size_t contentLength;
  ostringstream content;
  string error;
  bool flush;
};

  
} /* namespace shot */
