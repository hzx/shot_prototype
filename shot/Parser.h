#ifndef SHOT_PARSER_H
#define SHOT_PARSER_H


#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <iostream>
#include "Request.h"


using std::string;
using std::unordered_map;


namespace shot {


class Parser {
public:
  Parser();
  ~Parser();

  void parse(Request& request, string& source, size_t length);
  size_t parseHead(Request& request, string& source, size_t length);
  void processHead(Request& request);
  void parseProtocol(Request& request, string& source, size_t length);
  void parseMethod(Request& request, string& source, size_t length);
  void parseUrl(Request& request, string& source, size_t left, size_t right);
  void parseGetParams(Request& request, string& source,
      size_t left, size_t right);
  void parseVersion(Request& request, string& source, size_t left, size_t right);
  size_t parseHeaders(unordered_map<string, string>& headers, string& source,
      size_t left, size_t right);
  void parseCookie(unordered_map<string, string>& cookie, string& source);
  void parseContentDisposition(File& file, string& source);
  void parseBody(Request& request, string& source, size_t left, size_t right);
  size_t parseParams(unordered_map<string, string>& params, string& source,
      size_t left, size_t right);
  void parseBodyData(Request& request, string& source,
      size_t left, size_t right);
  size_t parseFileHead(File& file, string& source, size_t left, size_t right);
  size_t parseFileHeaders(File& file, string& source, size_t left, size_t right);
  size_t getPos(string& source, char c, size_t left, size_t right);
  size_t getPosBack(string& source, char c, size_t left, size_t right);
  size_t fixRNPos(string& source, size_t pos);
  size_t getMarkPos(string& source, string& mark, size_t left, size_t right);
  void setRequestBad(Request& request);
  void changeRequestProgress(Request& request, size_t length);
  void appendFile(File& file, string& source, size_t left, size_t right);
  void newRequestFile(Request& request);
};


} /* namespace shot */

#endif /* end of include guard: SHOT_PARSER_H */
