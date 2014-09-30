#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <iterator>
#include <openssl/md5.h>
#include "shot/utils.h"
#include "shot/Filer.h"


namespace shot {


Filer::Filer() {
}


Filer::~Filer() {
}


Filer& Filer::instance() {
  static Filer filer;
  return filer;
}


string Filer::loadCached(const char* filename) {
  // get from store content
  auto iter = cache.find(filename);

  // if not in cache, load it and place to cache
  if (iter == cache.end()) {
    auto content = pathLoad(filename);
    cache.insert({filename, content});
    iter = cache.find(filename);
  }

  return iter->second;
}


void Filer::save(const char* filename, string& content) {
}


// TODO: rewrite for files by chunks
/* string Filer::getHash(const char* filename) { */
/*   // find in hash cache */
/*   auto iter = hash.find(filename); */
/*   string gen; */

/*   if (iter == hash.end()) { */
/*     string content = load(filename); */

/*     // get hash from file content */
/*     unsigned char result[MD5_DIGEST_LENGTH]; */
/*     MD5((unsigned char*)content.c_str(), content.length(), result); */

/*     // copy hash result to string */
/*     std::ostringstream buf; */

/*     /1* buf << std::hex << std::setfill('0') << std::setw(2); *1/ */
/*     /1* std::copy(result, result + MD5_DIGEST_LENGTH, *1/ */
/*     /1*     std::ostream_iterator<unsigned int>(buf)); *1/ */

/*     buf << std::hex << std::setfill('0'); */
/*     for (int i = 0; i < MD5_DIGEST_LENGTH; ++i) { */
/*       buf << std::setw(2) << int(result[i]); */
/*     } */

/*     gen = buf.str(); */

/*     // add filename hash to cache */
/*     hash.insert({filename, gen}); */
/*   } else { */
/*     gen = iter->second; */
/*   } */

/*   return gen; */
/* } */


} /* namespace shot */
