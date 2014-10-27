#include "Logger.h"
#include <iostream>


namespace shot {


Logger::Logger() {
}


Logger::~Logger() {
}


Logger& Logger::instance() {
  static Logger logger;
  return logger;
}


void Logger::error(int errorCode, std::string errorMsg) {
  std::cerr << "[error " << errorCode << "] " << errorMsg << std::endl;
}


void Logger::write(std::string msg) {
  std::cout << msg << std::endl;
}


} /* namespace shot */
