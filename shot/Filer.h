#pragma once
#include <string>
#include <unordered_map>


using std::string;
using std::unordered_map;


namespace shot {


class Filer {
public:
  Filer();
  ~Filer();

  static Filer& instance();

  string loadCached(const char* filename);

private:
  // filename, content
  unordered_map<string, string> cache;
  // filename, hash
  unordered_map<string, string> hash;
};


} /* namespace shot */
