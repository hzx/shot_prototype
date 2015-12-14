#pragma once
#include <string>
#include <vector>
#include <sstream>


namespace shot {


class HtmlTemplate {
public:
  HtmlTemplate();
  std::string toString();

  std::string lang;
  std::string title;

  std::ostringstream head;
  std::ostringstream body;
};


} /* namespace shot */
