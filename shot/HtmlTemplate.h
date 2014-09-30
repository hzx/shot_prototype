#ifndef SHOT_HTML_TEMPLATE_H
#define SHOT_HTML_TEMPLATE_H


#include <string>
#include <vector>


namespace shot {


class HtmlTemplate {
public:
  HtmlTemplate();
  std::string toString();
  void writeHead(const char* txt);
  void writeHead(const std::string& txt);
  // writeBody
  void write(const char* txt);
  void write(const std::string& txt);

  std::string lang;
  std::string title;
  std::vector<std::string> head;
  std::vector<std::string> body;
};


} /* namespace shot */

#endif /* end of include guard: SHOT_HTML_TEMPLATE_H */
