#include "Options.h"
#include "Filer.h"
#include "utils.h"
#include "AppHandler.h"


namespace shot {


AppTemplate::AppTemplate(string title, string css, string js) {
  // add noscript redirect
  // inline css, js
  head <<
    "<noscript>"
    "<meta http-equiv=\"refresh\" content=\"0;url=/noscript\"/>"
    "</noscript>"
    "<title>" << title << "</title>"
    "<style type=\"text/css\">" << css << "</style>"
    "<script type=\"text/javascript\">" << js << "</script>";
}


void AppHandler::appGet(string title, string cssfilename, string jsfilename) {
  auto cssabs = shot::Options::instance().directory + cssfilename;
  auto jsabs = shot::Options::instance().directory + jsfilename;

  if (!shot::pathExists(cssabs.c_str()) ||
      !shot::pathExists(jsabs.c_str())) {
    response.content << "Что-то не нашел на сервере.";
    return;
  }

  auto css = shot::Filer::instance().loadCached(cssabs.c_str());
  auto js = shot::Filer::instance().loadCached(jsabs.c_str());

  AppTemplate templ(title, css, js);

  response.setHtmlHeader();
  response.content << templ.toString();
}


} /* namespace shot */
