#include <sstream>
#include <algorithm>
#include <iterator>
#include "SitemapTemplate.h"


namespace shot {


std::string SitemapIndexTemplate::toString() {
  std::ostringstream buf;

  buf <<
    "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
    "<sitemapindex xmlns=\"http://www.sitemaps.org/schemas/sitemap/0.9\">" <<
    content.str() <<
    "</sitemapindex>";

  return buf.str();
}


void SitemapIndexTemplate::setPrefix(const char* url) {
  prefix = url;
}


void SitemapIndexTemplate::write(const char* url) {
  content <<
    "<sitemap>"
      "<loc>" << prefix << url << "</loc>"
    "</sitemap>";
}


std::string SitemapUrlsetTemplate::toString() {
  std::ostringstream buf;

  buf <<
    "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
    "<urlset xmlns=\"http://www.sitemaps.org/schemas/sitemap/0.9\">" <<
    content.str() <<
    "</urlset>";

  return buf.str();
}


void SitemapUrlsetTemplate::setPrefix(const char* url) {
  prefix = url;
}


void SitemapUrlsetTemplate::write(const char* url) {
  content <<
    "<url>"
      "<loc>" << prefix << url << "</loc>"
    "</url>";
}

  
} /* namespace shot */

