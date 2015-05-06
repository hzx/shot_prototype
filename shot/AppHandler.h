#ifndef SHOT_APPHANDLER_H
#define SHOT_APPHANDLER_H


#include <string>
#include "HtmlTemplate.h"
#include "Handler.h"


using std::string;


namespace shot {


class AppTemplate: public shot::HtmlTemplate {
public:
  AppTemplate(string title, string js);
};
  

class AppHandler: public shot::Handler {
public:
  void appGet(string title, string jsfilename);
};


} /* namespace shot */


#endif /* end of include guard: SHOT_APPHANDLER_H */
