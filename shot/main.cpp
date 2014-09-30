#include <iostream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include "shot/Config.h"
#include "shot/Request.h"
#include "shot/Server.h"
#include "shot/HtmlTemplate.h"
#include "shot/Handler.h"
#include "shot/Filer.h"
#include "shot/utils.h"


using shot::factory;


// base html response
char msg[] = 
"HTTP/1.1 200 OK\n"
"Content-Type:text/html;charset=UTF-8\n"
"Server:shot\n"
"\n"
"<!doctype html>"
"<html>"
"<head>"
  "<meta charset=\"utf-8\"/>"
  "<script type=\"text/javascript\">"
    "window.location = \"/d#\" + window.location.pathname + window.location.search;"
  "</script>"
  "<title>Shot server - привет</title>"
"</head>"
"<body>"
  "<h1>Shot server - привет</h1>"
"</body>"
"</html>";


class App {
public:
  App();
  static App& instance();

  shot::Filer filer;
};


App::App() {
}


App& App::instance() {
  static App app;
  return app;
}


class BaseTemplate: public shot::HtmlTemplate {
public:
  BaseTemplate();
};


BaseTemplate::BaseTemplate() {
  lang = "ru";
  title = " - Test application";

  std::string appcss = shot::Config::instance().directory + "/static/app.css";
  std::string appjs = shot::Config::instance().directory + "/static/app.js";

  writeHead(
    "<script type=\"text/javascript\">"
      "window.location = \"/d#\" + window.location.pathname + window.location.search;"
    "</script>"
  );

  writeHead("<style type=\"text/css\">");
  writeHead(shot::Filer::instance().loadCached(appcss.c_str()));
  writeHead("</style>");

  writeHead("<script type=\"text/javascript\">");
  writeHead(shot::Filer::instance().loadCached(appjs.c_str()));
  writeHead("</script>");
}


class MainTemplate: public BaseTemplate {
 public:
  MainTemplate();
};


MainTemplate::MainTemplate() {
  writeHead(
    "<title>Главная" + title + "</title>"
  );

  write(
    "<h1>Главная</h1>"
  );
}


class MainHandler: public shot::Handler {
public:
  void get();
};


void MainHandler::get() {
  MainTemplate templ;

  response.setHtmlHeader();
  write(templ.toString());
}


class ContactTemplate: public BaseTemplate {
public:
  ContactTemplate();
};


ContactTemplate::ContactTemplate() {
  writeHead(
    "<title>Контакты" + title + "</title>"
  );

  write(
    "<h1>Контакты</h1>"
  );
}


class ContactHandler: public shot::Handler {
public:
  void get();
};


void ContactHandler::get() {
  ContactTemplate templ;

  response.setHtmlHeader();
  write(templ.toString());
}


class E404Template: public BaseTemplate {
public:
  E404Template();
};


E404Template::E404Template() {
  writeHead(
    "<title>Ошибка 404. Страница не найдена" + title + "</title>"
  );

  write(
    "<h1>Ошибка 404. Страница не найдена</h1>"
  );
}


class E404Handler: public shot::Handler {
public:
  void get();
};


void E404Handler::get() {
  E404Template templ;

  response.setHtmlHeader();
  write(templ.toString());
}


class E500Handler: public shot::Handler {
public:
  void get();
};


void E500Handler::get() {
  std::cout << "E500Handler not implemented" << std::endl;
}


/* class PublicApp { */
/* public: */
/*   static void mainHandler(shot::Request& request, shot::Response& response) { */
/*     MainTemplate templ; */

/*     response.setHtmlHeader(); */
/*     response.write(templ.toString()); */
/*   } */

/*   static void contactHandler(shot::Request& request, shot::Response& response) { */
/*     ContactTemplate templ; */

/*     response.setHtmlHeader(); */
/*     response.write(templ.toString()); */
/*   } */

/*   static void loginHandler(shot::Request& request, shot::Response& response) { */
/*     std::cout << "loginHandler not implemented" << std::endl; */
/*   } */

/*   static void logoutHandler(shot::Request& request, shot::Response& response) { */
/*     std::cout << "logoutHandler not implemented" << std::endl; */
/*   } */

/*   static void regHandler(shot::Request& request, shot::Response& response) { */
/*     std::cout << "regHandler not implemented" << std::endl; */
/*   } */

/*   static void changePasswordHandler(shot::Request& request, shot::Response& response) { */
/*     std::cout << "changePasswordHandler not implemented" << std::endl; */
/*   } */

/*   static void handler404(shot::Request& request, shot::Response& response) { */
/*     E404Template templ; */

/*     templ.body.push_back( */
/*         "<p>Страница по адресу \"" + request.url + "\" не найдена</p>" */
/*         ); */

/*     response.setHtmlHeader(); */
/*     response.write(templ.toString()); */
/*   } */

/*   static void handler500(shot::Request& request, shot::Response& response) { */
/*     std::cout << "handler500 not implemented" << std::endl; */
/*   } */

/*   static void faviconHandler(shot::Request& request, shot::Response& response) { */
/*     request.slugs.insert(request.slugs.begin(), "static"); */
/*     staticHandler(request, response); */
/*   } */

  /* static void staticHandler(shot::Request& request, shot::Response& response) { */
  /*   if (request.slugs.size() < 2) { */
  /*     PublicApp::handler404(request, response); */
  /*     return; */
  /*   } */

  /*   // compose filename without first slug + static folder prefix, */
  /*   // first slug route to this handler */
  /*   std::string filename = App::instance().config->directory + */
  /*     shot::joinUrlSlugs(request.slugs.begin(), request.slugs.end()); */

  /*   // TODO: check file exists */
  /*   if (!shot::Filer::exists(filename.c_str())) { */
  /*     PublicApp::handler404(request, response); */
  /*     return; */
  /*   } */

  /*   // set html header by extension */
  /*   std::string ext = shot::getExtension(filename); */
  /*   shot::ResponseUtils::setHeaderByExtension(response, ext); */

  /*   // add file to response */
  /*   response.write(App::instance().filer.loadCached(filename.c_str())); */
  /* } */
/* }; */


int main(int argc, const char *argv[]) {
  shot::Config& config = shot::Config::instance();

  int error = config.parseCmd(argc, argv);

  if (error != 0) {
    std::cout << "parse config cmd params error: " << error << std::endl;
    return error;
  }

  shot::Server server;

  shot::type_factories factories = {
    {"", factory<MainHandler>},
    {"contact", factory<ContactHandler>}
  };

  shot::Server::instance().setFactories(factories);

  /* server.setHandlerGet("", PublicApp::mainHandler); */
  /* server.setHandlerGet("contact", PublicApp::contactHandler); */
  /* server.setHandlerGet("login", PublicApp::loginHandler); */
  /* server.setHandlerGet("logout", PublicApp::logoutHandler); */
  /* server.setHandlerGet("reg", PublicApp::regHandler); */
  /* server.setHandlerGet("change-password", PublicApp::changePasswordHandler); */
  /* server.setHandlerGet("favicon.ico", PublicApp::faviconHandler); */
  /* server.setHandlerGet("static", PublicApp::staticHandler); */
  /* server.set404Handler(PublicApp::handler404); */
  /* server.set500Handler(PublicApp::handler500); */

  shot::Server::instance().run();

  return shot::Server::instance().getErrorCode();
}
