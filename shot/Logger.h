#ifndef SHOT_LOGGER_H
#define SHOT_LOGGER_H


#include <string>


namespace shot {


class Logger {
public:
  Logger();
  ~Logger();
  static Logger& instance();
  void error(int errorCode, std::string errorMsg);
  void write(std::string msg);
};


} /* namespace shot */

#endif /* end of include guard: SHOT_LOGGER_H */
