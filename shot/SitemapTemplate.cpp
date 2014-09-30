#include <sstream>
#include <algorithm>
#include <iterator>
#include "shot/SitemapTemplate.h"


namespace shot {


std::string SitemapIndexTemplate::toString() {
  std::ostringstream buf;

  buf <<
    "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
    "<sitemapindex xmlns=\"http://www.sitemaps.org/schemas/sitemap/0.9\">"
  ;

  std::copy(content.begin(), content.end(),
      std::ostream_iterator<std::string>(buf));

  buf <<
    "</sitemapindex>"
  ;

  return buf.str();
}


void SitemapIndexTemplate::setPrefix(const char* url) {
  prefix = url;
}


void SitemapIndexTemplate::write(const char* url) {
  write(std::string(url));
}


void SitemapIndexTemplate::write(const std::string& url) {
  content.push_back(
    "<sitemap>"
      "<loc>" + prefix + url + "</loc>"
    "</sitemap>"
  );
}


std::string SitemapUrlsetTemplate::toString() {
  std::ostringstream buf;

  buf <<
    "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
    "<urlset xmlns=\"http://www.sitemaps.org/schemas/sitemap/0.9\">"
  ;

  std::copy(content.begin(), content.end(),
      std::ostream_iterator<std::string>(buf));

  buf <<
    "</urlset>"
  ;

  return buf.str();
}


void SitemapUrlsetTemplate::setPrefix(const char* url) {
  prefix = url;
}


void SitemapUrlsetTemplate::write(const char* url) {
  write(std::string(url));
}


void SitemapUrlsetTemplate::write(const std::string& url) {
  content.push_back(
    "<url>"
      "<loc>" + prefix + url + "</loc>"
    "</url>"
  );
}

  
} /* namespace shot */

