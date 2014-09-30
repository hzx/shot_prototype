#ifndef SHOT_FILER_H
#define SHOT_FILER_H


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

  /* std::string load(const char* filename); */
  string loadCached(const char* filename);
  void save(const char* filename, string& content);
  /* std::string getHash(const char* filename); */

  /* static bool exists(const char* filename); */

private:
  // filename, content
  unordered_map<string, string> cache;
  // filename, hash
  unordered_map<string, string> hash;
};


} /* namespace shot */

#endif /* end of include guard: SHOT_FILER_H */
