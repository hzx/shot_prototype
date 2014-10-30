#ifndef SHOT_SYSTEM_HANDLERS_H
#define SHOT_SYSTEM_HANDLERS_H


#include "HtmlTemplate.h"
#include "Handler.h"


namespace shot {


class StaticHandler: public Handler {
public:
  void staticGet();
  void get();
  void xget();
};


class FaviconHandler: public StaticHandler {
public:
  void get();
};


class RobotsHandler: public StaticHandler {
public:
  void get();
};


class NoscriptTemplate: public HtmlTemplate {
public:
  NoscriptTemplate();
};


class NoscriptHandler: public Handler {
public:
  void get();
};


class RedirectHandler: public Handler {
public:
  RedirectHandler(string location);
  void get();
  void xget();
  void post();
  void xpost();
private:
  string location;
};


class E400Template: public HtmlTemplate {
public:
  E400Template();
};


class E400Handler: public Handler {
public:
  E400Handler();
  void get();
  void xget();
};


class E403Template: public HtmlTemplate {
public:
  E403Template();
};


class E403Handler: public Handler {
public:
  E403Handler();
  void get();
  void xget();
};


class E404Template: public HtmlTemplate {
public:
  E404Template();
};


class E404Handler: public Handler {
public:
  E404Handler();
  void get();
  void xget();
};


class E406Template: public HtmlTemplate {
public:
  E406Template();
};


class E406Handler: public Handler {
public:
  E406Handler();
  void get();
  void xget();
};


class E500Template: public HtmlTemplate {
public:
  E500Template();
};


class E500Handler: public Handler {
public:
  E500Handler();
  void get();
  void xget();
};

  
} /* namespace shot */


#endif /* end of include guard: SHOT_SYSTEM_HANDLERS_H */
