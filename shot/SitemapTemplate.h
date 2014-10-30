#ifndef SHOT_SITEMAPTEMPLATE_H
#define SHOT_SITEMAPTEMPLATE_H


#include <string>
#include <vector>
#include <sstream>


namespace shot {


class SitemapIndexTemplate {
public:
  std::string toString();
  void setPrefix(const char* url);
  void write(const char* url);
  std::string prefix;
  std::ostringstream content;
};


class SitemapUrlsetTemplate {
public:
  std::string toString();
  void setPrefix(const char* url);
  void write(const char* url);
  std::string prefix;
  std::ostringstream content;
};

  
} /* namespace shot */


#endif /* end of include guard: SHOT_SITEMAPTEMPLATE_H */
