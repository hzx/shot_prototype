#include <cstring>
#include <algorithm>
#include <regex>
#include <iostream>
#include <fstream>
#include <iterator>
#include <cctype>
#include "Logger.h"
#include "Parser.h"
#include "Options.h"
#include "http.h"
#include "utils.h"


namespace shot {


Parser::Parser() {
}


Parser::~Parser() {
}


void Parser::parse(Request& request, string& source, size_t length) {
  size_t hpos = parseHead(request, source, length);
  if (request.isBad) return;

  processHead(request);

  parseBody(request, source, hpos + 1, length);
}


// init content-type, content-length, parse cookie
void Parser::processHead(Request& request) {
  // get cookie from headers
  auto ci = request.headers.find(KL_COOKIE);
  if (ci != request.headers.end()) { 
    // parse cookie header value
    parseCookie(request.cookie, ci->second);
  }

  if (request.method != C_POST) return;

  // set host from headers
  auto hi = request.headers.find(KL_HOST);
  if (hi == request.headers.end()) {
    setRequestBad(request);
    return;
  }

  request.host = hi->second;

  // check xsrf
  if (!request.checkXsrf()) { // forgery
    setRequestBad(request);
    return;
  }

  // init content type
  string contentType = request.getHeader(KL_CONTENT_TYPE);
  if (std::memcmp(contentType.c_str(), V_FORM_URLENCODED, std::strlen(V_FORM_URLENCODED)) == 0) {
    request.contentType = C_FORM_URLENCODED;
  } else {
    if (std::memcmp(contentType.c_str(), V_FORM_DATA, std::strlen(V_FORM_DATA)) == 0) {
      request.contentType = C_FORM_DATA;
    } else {
      // unknown or unsupported content-type
      setRequestBad(request);
      return;
    }
  }

  // init content length
  string contentLength = request.getHeader(KL_CONTENT_LENGTH);
  if (!contentLength.empty()) {
    try {
      request.contentLength = std::stol(contentLength);
    } catch (...) {
      request.contentLength = 0;
    }
  }
}


// return \n position in blank line (+1 is body symbols)
size_t Parser::parseHead(Request& request, string& source, size_t length) {
  size_t npos = getPos(source, '\n', 0, length);

  if (npos == length) { // we dont have data even for protocol
    setRequestBad(request);
    return length;
  }

  parseProtocol(request, source, fixRNPos(source, npos));
  if (request.isBad) return length;

  npos = parseHeaders(request.headers, source, npos + 1, length);
  return npos;
}


void Parser::parseProtocol(Request& request, string& source, size_t length) {
  size_t methodSpace = getPos(source, ' ', 0, length);
  size_t urlSpace = getPos(source, ' ', methodSpace + 1, length);

  if (methodSpace == length or urlSpace == length) {
    request.isBad = true;
    return;
  }

  parseMethod(request, source, methodSpace);
  if (request.isBad) return;

  parseUrl(request, source, methodSpace + 1, urlSpace);
  if (request.isBad) return;
  request.url.assign(source, methodSpace + 1, urlSpace - (methodSpace + 1));

  parseVersion(request, source, urlSpace + 1, length);
  request.version.assign(source, urlSpace + 1, length - (urlSpace + 1));
}


// return next position to parse
void Parser::parseMethod(Request& request, string& source, size_t length) {
  if (length == 3 and std::memcmp(source.c_str(), H_GET, 3) == 0) {
    request.method = C_GET;
    request.isDone = true; // get request always mark as done
    return;
  } 

  if (length == 4 and std::memcmp(source.c_str(), H_POST, 4) == 0) {
    request.method = C_POST;
    return;
  } 
  
  // unknown or unsupported request method
  setRequestBad(request);
}


void Parser::parseUrl(Request& request, string& source, size_t left, size_t right) {
  if (right - left < 1 or source[left] != '/') {
    setRequestBad(request);
    return;
  }

  // set right until ?
  right = getPos(source, '?', left + 1, right);

  // find left and right slug positions until right (space or ?)
  size_t lpos = left + 1;
  size_t rpos;
  for (;;) {
    rpos = getPos(source, '/', lpos, right);

    string slug(source, lpos, rpos - lpos);
    if (slug.length() > 0) request.slugs.push_back(slug);

    if (rpos == right) {
      break;
    }

    lpos = rpos + 1;
  }

  if (request.slugs.size() == 0) {
    request.slugs.push_back("");
  }
}


void Parser::parseVersion(Request& request, string& source, size_t left, size_t right) {
  // TODO: use it
}


size_t Parser::parseHeaders(unordered_map<string, string>& headers,
    string& source, size_t left, size_t right) {
  size_t lpos = left;
  size_t vpos; // begin value position
  size_t rpos; // end value position
  size_t cpos; // colon position

  for (;;) {
    rpos = getPos(source, '\n', lpos, right);
    cpos = getPos(source, ':', lpos, rpos);
    if (cpos == rpos) {
      return rpos;
    }

    vpos = cpos + 1;

    // skip space in value  beginning
    if (source[vpos] == ' ') ++vpos;

    string name(source, lpos, cpos - lpos);
    string value(source, vpos, fixRNPos(source, rpos) - vpos);
    toLower(name); // some browsers sent lower case header names
    headers.insert({name, value});

    lpos = rpos + 1;
  }

  return right;
}


void Parser::parseCookie(unordered_map<string, string>& cookie, string& source) {
  size_t right = source.length();
  size_t lpos = 0; // name begin position
  size_t vpos; // begin value position
  size_t rpos; // end value position
  size_t epos; // equal position

  for (;;) {
    rpos = getPos(source, ';', lpos, right);
    epos = getPos(source, '=', lpos, rpos);
    if (epos == rpos) break;

    vpos = epos + 1;
    
    // skip space in name beginning
    if (source[lpos] == ' ') ++lpos;

    string name(source, lpos, epos - lpos);
    string value(source, vpos, rpos - vpos);
    cookie.insert({name, value});

    lpos = rpos + 1;
  }
}


void Parser::parseContentDisposition(File& file, string& source) {
  size_t right = source.length();
  // skip form-data to next ;
  size_t lpos = getPos(source, ';', 0, right) + 1;
  size_t rpos;
  size_t epos; // equal position
  size_t vpos; // value begin position

  for (;;) {
    rpos = getPos(source, ';', lpos, right);
    epos = getPos(source, '=', lpos, rpos);

    if (epos == rpos) break;

    vpos = epos + 1;

    // skip space in name beginning
    if (source[lpos] == ' ') ++lpos;

    if (std::memcmp("name", source.c_str() + lpos, 4) == 0) {
      file.name.assign(source, vpos, rpos - vpos);
      file.name = unquote(file.name);
    } else {
      if (std::memcmp("filename", &source[lpos], 8) == 0) {
        file.filename.assign(source, vpos, rpos - vpos);
        file.filename = unquote(file.filename);
      }
    }

    lpos = rpos + 1;
  }
}


void Parser::parseBody(Request& request, string& source,
    size_t left, size_t right) {
  if (request.method != C_POST) return;

  // for short post request done
  if (left == right and request.contentLength == 0) {
    request.isDone = true;
    return;
  }

  switch (request.contentType) {
    case C_FORM_URLENCODED:
      parseParams(request.params, source, left, right);
      changeRequestProgress(request, right - left);
      break;
    case C_FORM_DATA:
      parseBodyData(request, source, left, right);
      changeRequestProgress(request, right - left);
      break;
    default:
      setRequestBad(request);
      break;
  }
}


size_t Parser::parseParams(unordered_map<string, string>& params,
    string& source, size_t left, size_t right) {
  size_t lpos = left;
  size_t rpos;
  size_t epos;
  for (;;) {
    rpos = getPos(source, '&', lpos, right);
    epos = getPos(source, '=', lpos, rpos);
    if (epos == rpos) {
      return rpos;
    }

    string name(source, lpos, epos - lpos);
    string value(source, epos + 1, rpos - (epos + 1));
    params.insert({name, decodeUrl(value)});

    lpos = rpos + 1;
  }
  return right;
}


void Parser::parseBodyData(Request& request, string& source,
    size_t left, size_t right) {
  size_t pos = left;
  size_t mpos; // mark position;
  size_t npos; // \n position
  size_t bpos; // headers begin position
  size_t hpos; // headers end position - blank line \n

  for (;;) {
    if (pos >= right) break;

    // parse file headers
    if (!request.file.isCreated) {
      if (request.mark.empty()) {
        // remember mark in begin line
        npos = getPos(source, '\n', pos, right);
        if (npos == right) { // dont have right protocol structure
          setRequestBad(request);
          return;
        }

        request.mark.assign(source, pos, fixRNPos(source, npos) - pos);

        bpos = npos + 1;
      } else {
        bpos = pos;
      }

      hpos = parseFileHeaders(request.file, source, bpos, right);
      request.file.path = Options::instance().tmp + randomFilename(request.file.filename);

      // move position after headers and blank line to data
      pos = hpos + 1;

      request.file.isCreated = true;
    }

    // find close mark
    mpos = getMarkPos(source, request.mark, pos, right);

    appendFile(request.file, source, pos, mpos);

    if (mpos == right) { // mark not found, write all
      pos = right;
    } else { // mark found, write all, prepare parse for new file
      // set position after \n in mark line
      pos = getPos(source, '\n', mpos, right) + 1;
      newRequestFile(request);
    }
  }
}


size_t Parser::parseFileHeaders(File& file, string& source,
    size_t left, size_t right) {
  unordered_map<string, string> headers;

  size_t hpos = parseHeaders(headers, source, left, right);

  // init file content-type
  auto it = headers.find(KL_CONTENT_TYPE);
  if (it != headers.end()) {
    file.type = it->second;
  }

  // init file fields
  auto di = headers.find(KL_CONTENT_DISPOSITION);
  if (di != headers.end()) {
    parseContentDisposition(file, di->second);
  }

  return hpos;
}


size_t Parser::getPos(string& source, char c, size_t left, size_t right) {
  for (size_t i = left; i < right; ++i) {
    if (source[i] == c) {
      return i;
    }
  }

  return right;
}


size_t Parser::getPosBack(string& source, char c, size_t left, size_t right) {
  for (size_t i = right - 1; i > left; --i) {
    if (source[i] == c) {
      return i;
    }
  }

  return left;
}


size_t Parser::fixRNPos(string& source, size_t pos) {
  return source[pos - 1] == '\r' ? pos - 1 : pos;
}


size_t Parser::getMarkPos(string& source, string& mark, size_t left, size_t right) {
  size_t markLength = mark.length();

  for (size_t i = left; i < right; ++i) {
    if (source[i] == mark[0]) { // find first symbol match
      if (std::memcmp(&source[i], &mark[0], markLength) == 0) {
        return i;
      }
    }
  }

  return right;
}


void Parser::setRequestBad(Request& request) {
  request.isDone = true;
  request.isBad = true;
}


void Parser::changeRequestProgress(Request& request, size_t length) {
  request.contentProgress += length;
  if (request.contentLength == 0 or 
      request.contentProgress >= request.contentLength) {
    request.isDone = true;
  }
}


void Parser::appendFile(File& file, string& source, size_t left, size_t right) {
  std::ofstream out(file.path.c_str(), std::ios::out | std::ios::app);
  out.write(&source[left], right - left);
  out.close();
}


void Parser::newRequestFile(Request& request) {
  request.files.push_back(request.file);

  request.file.name = "";
  request.file.filename = "";
  request.file.type = "";
  //  file parse state
  request.file.isCreated = false;
  request.file.path = "";
}


} /* namespace shot */
