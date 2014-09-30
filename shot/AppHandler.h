#ifndef SHOT_APPHANDLER_H
#define SHOT_APPHANDLER_H


#include <string>
#include "shot/HtmlTemplate.h"
#include "shot/Handler.h"


using std::string;


namespace shot {


class AppTemplate: public shot::HtmlTemplate {
public:
  AppTemplate(string title, string css, string js);
};
  

class AppHandler: public shot::Handler {
public:
  void appGet(string title, string cssfilename, string jsfilename);
};


} /* namespace shot */


#endif /* end of include guard: SHOT_APPHANDLER_H */
