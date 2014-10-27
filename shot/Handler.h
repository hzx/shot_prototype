#ifndef SHOT_HANDLER_H
#define SHOT_HANDLER_H


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

  string getXsrf();
  void setXsrf();
  void setError(int status, string message);
  void redirectPermanently(string location);
  void redirectTemporarily(string location);
  void write(const char* txt);
  void write(std::string txt);
  void flush();

  int socket;
  Request* request;
  Response response;

  int state;
  string xsrf;
};


} /* namespace shot */

#endif /* end of include guard: SHOT_HANDLER_H */
