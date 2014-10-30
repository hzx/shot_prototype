#include "utils.h"
#include "Filer.h"
#include "Config.h"
#include "http.h"
#include "system_handlers.h"
#include "Logger.h"


using std::string;


namespace shot {


void StaticHandler::staticGet() {
  if (request->slugs.size() < 2) {
    response.status = HTTP_404;
    return;
  }

  string filename = Config::instance().directory +
    joinUrlSlugs(request->slugs.begin(), request->slugs.end());

  // TODO: set config static directory and check filename child
  if (!pathExists(filename.c_str())) {
    response.status = HTTP_404;
    return;
  }

  // set html header by extension
  string ext = getExtension(filename);
  /* ResponseUtils::setHeaderByExtension(this->response, ext); */
  string extHeader = getExtHeader(ext);
  if (!extHeader.empty()) {
    response.setHeader(K_CONTENT_TYPE, extHeader.data());
  }


  response.content << Filer::instance().loadCached(filename.c_str());
}


void StaticHandler::get() {
  staticGet();
}


void StaticHandler::xget() {
  staticGet();
}


void FaviconHandler::get() {
  request->slugs.insert(request->slugs.begin(), "static");
  staticGet();
}


void RobotsHandler::get() {
  request->slugs.insert(request->slugs.begin(), "static");
  staticGet();
}


NoscriptTemplate::NoscriptTemplate() {
  lang = "ru";
  title = "Браузер не поддерживает javascript или javascript отключен";

  head << 
    "<title>Браузер не поддерживает javascript или javascript отключен</title>";
  body <<
  "<h1>Браузер не поддерживает javascript или javascript отключен</h1>"
  "<p>Для нормальной работы с сайтом требуется включить javascript или установить браузер, который его поддерживает</p>"
  "<p>"
    "<p><a href=\"http://www.google.com/chrome\">Chrome</a></p>"
    "<p><a href=\"http://www.mozilla.org/firefox\">Firefox</a></p>"
    "<p><a href=\"http://www.opera.com\">Opera</a></p>"
  "</p>";
}


void NoscriptHandler::get() {
  NoscriptTemplate templ;

  response.setHtmlHeader();
  response.content << templ.toString();
}


RedirectHandler::RedirectHandler(string location_) {
  location = location_;
}


void RedirectHandler::get() {
  redirectTemporarily(location);
}


void RedirectHandler::xget() {
  redirectTemporarily(location);
}


void RedirectHandler::post() {
  redirectTemporarily(location);
}


void RedirectHandler::xpost() {
  redirectTemporarily(location);
}


E400Template::E400Template() {
  head << "<title>Ошибка 400. Неверный запрос</title>";
  body << "<h1>Ошибка 400. Неверный запрос</h1>";
}


E400Handler::E400Handler() {
  response.status = HTTP_400;
};


void E400Handler::get() {
  E404Template templ;

  response.setHtmlHeader();
  response.content << templ.toString();
}


void E400Handler::xget() {
  response.setTextHeader();
  response.content << "Ошибка 400. Неверный запрос. " + response.error;
}


E403Template::E403Template() {
  head << "<title>Ошибка 403. В доступе отказано</title>";
  body << "<h1>Ошибка 403. В доступе отказано</h1>";
}


E403Handler::E403Handler() {
  response.status = HTTP_403;
}


void E403Handler::get() {
  E403Template templ;

  if (!response.error.empty()) {
    response.content << "<p>" << response.error << "</p>";
  }

  response.setHtmlHeader();
  response.content << templ.toString();
}


void E403Handler::xget() {
  response.setTextHeader();
  response.content << "Ошибка 403. В доступе отказано. " << response.error;
}


E404Template::E404Template() {
  head << "<title>Ошибка 404. Страница не найдена</title>";
  body << "<h1>Ошибка 404. Страница не найдена</h1>";
}


E404Handler::E404Handler() {
  response.status = HTTP_404;
}
  

void E404Handler::get() {
  E404Template templ;

  response.setHtmlHeader();
  response.content << templ.toString();
}


void E404Handler::xget() {
  response.setTextHeader();
  response.content << "Ошибка 404. Страница не найдена. " + response.error;
}


E500Template::E500Template() {
  head << "<title>Ошибка 500. Что-то пошло не так.</title>";
  body << "<h1>Ошибка 500. Что-то пошло не так.</h1>"
    "<p>Обратитесь к разработчику</p>";
}


E500Handler::E500Handler() {
  response.status = HTTP_500;
}


void E500Handler::get() {
  E500Template templ;

  response.setHtmlHeader();
  response.content << templ.toString();
}


void E500Handler::xget() {
  response.setTextHeader();
  response.content << "Ошибка 500. Что-то пошло не так.";
}


} /* namespace shot */
