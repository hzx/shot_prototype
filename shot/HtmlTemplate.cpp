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

  buf <<
    "<!doctype html>"
    "<html lang=\"" << lang << "\">"
    "<head>"
    "<meta charset=\"utf-8\"/>" <<
    head.str() <<
    "</head>"
    "<body>" << body.str() <<
    "</body>"
    "</html>";

  return buf.str();
}


} /* namespace shot */
