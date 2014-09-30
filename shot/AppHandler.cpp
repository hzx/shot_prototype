#include "shot/Config.h"
#include "shot/Filer.h"
#include "shot/utils.h"
#include "shot/AppHandler.h"


namespace shot {


AppTemplate::AppTemplate(string title, string css, string js) {
  // add noscript redirect
  // inline css, js
  head.push_back(
    "<noscript>"
    "<meta http-equiv=\"refresh\" content=\"0;url=/noscript\"/>"
    "</noscript>"
    "<title>" + title + "</title>"
    "<style type=\"text/css\">" + css + "</style>"
    "<script type=\"text/javascript\">" + js + "</script>"
  );
}


void AppHandler::appGet(string title, string cssfilename, string jsfilename) {
  auto cssabs = shot::Config::instance().directory + cssfilename;
  auto jsabs = shot::Config::instance().directory + jsfilename;

  if (!shot::pathExists(cssabs.c_str()) ||
      !shot::pathExists(jsabs.c_str())) {
    write("Что-то не нашел на сервере.");
    return;
  }

  auto css = shot::Filer::instance().loadCached(cssabs.c_str());
  auto js = shot::Filer::instance().loadCached(jsabs.c_str());

  AppTemplate templ(title, css, js);

  response.setHtmlHeader();
  write(templ.toString());
}


} /* namespace shot */
