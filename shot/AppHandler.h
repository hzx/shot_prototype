#pragma once
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
