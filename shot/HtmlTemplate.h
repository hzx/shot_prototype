#ifndef SHOT_HTML_TEMPLATE_H
#define SHOT_HTML_TEMPLATE_H


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

#endif /* end of include guard: SHOT_HTML_TEMPLATE_H */
