#ifndef SHOT_RESPONSE_H
#define SHOT_RESPONSE_H


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
  void setStatus(int statuz);

  int status;
  unordered_map<string, string> headers;
  unordered_map<string, string> cookie;
  size_t contentLength;
  /* vector<string> content; */
  ostringstream content;
  string error;
  bool flush;
};

  
} /* namespace shot */


#endif /* end of include guard: SHOT_RESPONSE_H */
