#include <cstring>
#include <algorithm>
#include <regex>
#include <iostream>
#include <iterator>
#include <cctype>
#include "shot/Logger.h"
#include "shot/Parser.h"
#include "shot/http.h"
#include "shot/utils.h"


namespace shot {


Parser::Parser() {
}


Parser::~Parser() {
}


void Parser::initRequest(Request& request, string&& source) {

  // parse method
  auto firstEnd = std::find(source.begin(), source.end(), '\n');
  if (firstEnd == source.end()) { // not found first line
    return;
  }

  // parse method
  auto end = std::find(source.begin(), firstEnd, ' ');
  if (end == firstEnd) { // not found first space
    return;
  }

  string method(source.begin(), end);

  if (std::memcmp(method.c_str(), H_GET, 3) == 0) {
    request.method = C_GET;
    request.isDone = true;
  } else {
    if (std::memcmp(method.c_str(), H_POST, 4) == 0) {
      request.method = C_POST;
    } else {
      request.isDone = true;
      return;
    }
  }

  // parse url
  auto begin = end + 1;
  end = std::find(begin, firstEnd, ' ');
  if (end == firstEnd) { // not found second space
    return;
  }
  auto quest = std::find(begin, end, '?');

  request.url.assign(begin, quest);

  // parse version
  request.version.assign(end + 1, firstEnd);

  // split url by slugs
  auto current = std::find(request.url.begin(), request.url.end(), '/');
  if (current == request.url.end()) {
    return;
  }
  ++current;
  for (;;) {
    auto next = std::find(current, request.url.end(), '/');

    if (next != request.url.end()) {
      string slug(current, next);
      request.slugs.push_back(slug);
      current = next + 1;
    } else {
      string slug(current, next);
      request.slugs.push_back(slug);
      break;
    }
  }


  // parse headers
  current = firstEnd + 1;
  for (;;) {
    auto lineEnd = std::find(current, source.end(), '\n');
    if (lineEnd == source.end()) { // not found
      return;
    }

    if (lineEnd - current == 1) {
      break;
    }

    auto delim = std::find(current, lineEnd, ':');
    if (delim == lineEnd) {
      break;
    }

    string name(current, delim);
    string value(delim + 2, lineEnd-1);
    toLower(name);
    request.headers.insert({name, value});

    current = lineEnd + 1;
  }

  // set host
  auto hostit = request.headers.find(KL_HOST);
  if (hostit == request.headers.end()) {
    request.isBad = true;
    return;
  }
  request.host = hostit->second;

  // skip \n in last header
  ++current;

  // parse cookie
  auto cookieIter = request.headers.find(KL_COOKIE);
  if (cookieIter != request.headers.end()) {
    auto cookie = cookieIter->second;
    // parse name - between begin and =
    // parse value - between = and & (or end)
    auto cook = cookie.begin();
    for (;;) {
      if (*cook == ' ') {
        ++cook;
      }
      auto nameEnd = std::find(cook, cookie.end(), '=');
      if (nameEnd == cookie.end()) break;
      auto valueEnd = std::find(nameEnd + 1, cookie.end(), ';');
      auto valueBegin = nameEnd + 1;
      if (*valueBegin == ' ') {
        ++valueBegin;
      }

      string name(cook, nameEnd);
      string value(valueBegin, valueEnd);
      request.cookie.insert({name, value});

      if (valueEnd == cookie.end()) break;
      cook = valueEnd + 1;
    }
  }


  // parse post body
  if (request.method == C_POST) {
    if (!request.checkXsrf()) { // not valid post
      request.isBad = true;
      request.isDone = true;
      return;
    }

    auto blank = std::find(current, source.end(), '\n');

    if (blank != source.end()) {
      ++blank;

      parseBody_(request, blank, source.end());
    }
  } else {
    request.isDone = true;
  }
}


void Parser::parseBody(Request& request, string&& source) {
  parseBody_(request, source.begin(), source.end());
}


void Parser::parseBody_(Request& request,
    std::string::iterator begin, std::string::iterator end) {
  std::cout << "parseBody_:" << std::endl;

  // get Content-Length header
  string contentLength = request.getHeader(KL_CONTENT_LENGTH);
  string contentType = request.getHeader(KL_CONTENT_TYPE);
  size_t length = 0;

  if (!contentLength.empty()) {
    try {
      length = std::stol(contentLength);
    } catch (...) {
      length = 0;
    }
  }

  size_t bodyLength = end - begin;

  if (length == 0 or length == bodyLength) {
    request.isDone = true;
  }

  if (bodyLength <= 0) {
    return;
  }

  std::cout << "length: " << length << ", bodyLength: " << bodyLength << std::endl;

  // clean content type value
  auto it = std::find(contentType.begin(), contentType.end(), ';');
  if (it != contentType.end()) {
    contentType.assign(contentType.begin(), it);
  }

  if (contentType.compare(V_FORM_URLENCODED) == 0) {
    parseFormUrlencoded(request, begin, end);
    return;
  }

  if (contentType.compare(V_FORM_DATA) == 0) {
    parseFormData(request, begin, end);
    return;
  }

  // set request is bad and done
  request.isBad = true;
  request.isDone = true;
}


void Parser::parseFormUrlencoded(Request& request,
    std::string::iterator begin, std::string::iterator end) {
  auto current = begin;
  for (;;) {
    auto nameEnd = std::find(current, end, '=');
    if (nameEnd == end) break;
    auto valueEnd = std::find(nameEnd + 1, end, '&');
    auto valueBegin = nameEnd + 1;

    string name(current, nameEnd);
    string value(valueBegin, valueEnd);
    request.params.insert({name, decodeUrl(value)});

    if (valueEnd == end) break;
    current = valueEnd + 1;
  }
}


void Parser::parseFormData(Request& request,
    std::string::iterator begin, std::string::iterator end) {
}


} /* namespace shot */
