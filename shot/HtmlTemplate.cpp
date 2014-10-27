#include <sstream>
#include <algorithm>
#include <iterator>
#include "HtmlTemplate.h"


namespace shot {


HtmlTemplate::HtmlTemplate()
    : lang("ru") {
}


std::string HtmlTemplate::toString() {
  std::ostringstream buf;
  /* std::vector<std::string> pices; */

  // open html and head
  buf <<
    "<!doctype html>"
    "<html lang=\"" << lang << "\">"
    "<head>"
    "<meta charset=\"utf-8\"/>"
  ;

  // copy head
  /* std::copy(head.begin(), head.end(), std::back_inserter(pices)); */
  std::copy(head.begin(), head.end(), std::ostream_iterator<std::string>(buf));

  // close head and open body
  buf <<
    "</head>"
    "<body>"
  ;
  
  // copy body
  /* std::copy(body.begin(), body.end(), std::back_inserter(pices)); */
  std::copy(body.begin(), body.end(), std::ostream_iterator<std::string>(buf));

  // close body and html
  buf <<
    "</body>"
    "</html>"
  ;

  // copy pices to buf
  /* std::copy(pices.begin(), pices.end(), std::ostream_iterator<std::string>(buf)); */

  return buf.str();
}


void HtmlTemplate::writeHead(const char* txt) {
  head.push_back(txt);
}


void HtmlTemplate::writeHead(const std::string& txt) {
  head.push_back(txt);
}


void HtmlTemplate::write(const char* txt) {
  body.push_back(txt);
}


void HtmlTemplate::write(const std::string& txt) {
  body.push_back(txt);
}


} /* namespace shot */
