#pragma once
#include <string>
#include <vector>
#include <unordered_map>


using std::string;
using std::vector;
using std::unordered_map;


namespace shot {


class File {
public:
  File();

  string name;
  string filename;
  string type;

  //  file parse state
  bool isCreated;
  string path;
};


class Request {
public:
  Request();
  ~Request();

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
  char contentType;
  size_t contentLength;
  size_t contentProgress;

  // files
  vector<File> files;
  File file;
  string mark; // mark symbols, divide files like tag

  bool isKeepAlive();
  bool isXhr();
  bool checkXsrf();
  string getHeader(const char* name);
  string getCookie(const char* name);
  string getParam(const char* name);
  string getSecureCookie(const char* name);

  bool keepFiles;

private:
  void removeFiles();
};


} /* namespace shot */
