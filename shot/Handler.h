#pragma once
#include <memory>
#include "Request.h"
#include "Response.h"


namespace shot {


const int HANDLER_STATE_NONE = 0;
const int HANDLER_STATE_HEAD = 1;
const int HANDLER_STATE_BODY = 2;


class Handler {
public:
  Handler();
  virtual ~Handler();

  virtual void get();
  virtual void xget();
  virtual void post();
  virtual void xpost();
  virtual void del();
  virtual void xdel();

  string getXsrf();
  void setXsrf();
  void setError(int status, string message);
  void redirectPermanently(string location);
  void redirectTemporarily(string location);
  void flush();

  int socket;
  Request* request;
  Response response;

  int state;
  string xsrf;
};


} /* namespace shot */
