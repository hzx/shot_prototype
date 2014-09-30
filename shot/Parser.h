#ifndef SHOT_PARSER_H
#define SHOT_PARSER_H


#include <memory>
#include <sstream>
#include <string>
#include "shot/Request.h"


using std::string;


namespace shot {


class Parser {
public:
  Parser();
  ~Parser();

  void initRequest(Request& request, string&& source);
  void parseBody(Request& request, string&& source);
private:
  void parseBody_(Request& request, 
      std::string::iterator begin, std::string::iterator end);
  void parseFormUrlencoded(Request& request,
      std::string::iterator begin, std::string::iterator end);
  void parseFormData(Request& request,
      std::string::iterator begin, std::string::iterator end);
};


} /* namespace shot */

#endif /* end of include guard: SHOT_PARSER_H */
