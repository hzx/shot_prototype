#ifndef SHOT_REQUEST_H
#define SHOT_REQUEST_H


#include <string>
#include <vector>
#include <unordered_map>


using std::string;
using std::vector;
using std::unordered_map;


namespace shot {


class Request {
public:
  Request();
  ~Request();

  // for debug only
  /* string protocol; */
  
  bool isBad;
  bool isDone;
  string host;
  int method;
  string url;
  string version;
  vector<string> slugs;
  unordered_map<string, string> headers;
  unordered_map<string, string> cookie;
  unordered_map<string, string> params;
  // files


  bool isKeepAlive();
  bool isXhr();
  bool checkXsrf();
  string getHeader(const char* name);
  string getCookie(const char* name);
  string getParam(const char* name);
  string getSecureCookie(const char* name);

private:
};


} /* namespace shot */

#endif /* end of include guard: SHOT_REQUEST_H */
