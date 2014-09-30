#ifndef SHOT_SITEMAPTEMPLATE_H
#define SHOT_SITEMAPTEMPLATE_H


#include <string>
#include <vector>


namespace shot {


class SitemapIndexTemplate {
public:
  std::string toString();
  void setPrefix(const char* url);
  void write(const char* url);
  void write(const std::string& url);
private:
  std::string prefix;
  std::vector<std::string> content;
};


class SitemapUrlsetTemplate {
public:
  std::string toString();
  void setPrefix(const char* url);
  void write(const char* url);
  void write(const std::string& url);
private:
  std::string prefix;
  std::vector<std::string> content;
};

  
} /* namespace shot */


#endif /* end of include guard: SHOT_SITEMAPTEMPLATE_H */
