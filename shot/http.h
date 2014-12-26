#ifndef SHOT_HTTP_H
#define SHOT_HTTP_H


namespace shot {


// H_**** - header
// C_**** - code
// K_**** - key
// V_**** - value

const int HTTP_HEAD_MIN_SIZE = 14;
const char XSRF[] = "_xsrf";
const int MAX_AGE_DAYS = 30;

const char H_1_1[] = "HTTP/1.1";
const char H_GET[] = "GET";
const char H_POST[] = "POST";
const int C_NONE = 0;
const int C_GET = 1;
const int C_POST = 2;
const char K_CONTENT_LENGTH[] = "Content-Length";
const char KL_CONTENT_LENGTH[] = "content-length";
const char K_CONTENT_LENGTH_COLON[] = "Content-Length:";
const char K_CONTENT_DISPOSITION[] = "Content-Disposition";
const char KL_CONTENT_DISPOSITION[] = "content-disposition";
const char K_CONTENT_TYPE[] = "Content-Type";
const char KL_CONTENT_TYPE[] = "content-type";
const char K_SET_COOKIE[] = "Set-Cookie";
const char KL_SET_COOKIE[] = "set-cookie";
const char K_CONNECTION[] = "Connection";
const char KL_CONNECTION[] = "connection";
const char K_HOST[] = "Host";
const char KL_HOST[] = "host";
const char K_SERVER[] = "Server";
const char K_XHR[] = "X-Requested-With";
const char KL_XHR[] = "x-requested-with";
const char K_XSRF[] = "X-Xsrftoken";
const char KL_XSRF[] = "x-xsrftoken";
const char K_HASH[] = "X-Hash";
const char KL_HASH[] = "x-hash";
const char K_COOKIE[] = "Cookie";
const char KL_COOKIE[] = "cookie";
const char K_LOCATION[] = "Location";
const char V_TEXT_UTF8[] = "text/plain;charset=UTF-8";
const char V_HTML_UTF8[] = "text/html;charset=UTF-8";
const char V_XML_UTF8[] = "text/xml;charset=UTF-8";
const char V_CSS_UTF8[] = "text/css;charset=UTF-8";
const char V_JS_UTF8[] = "text/javascript;charset=UTF-8";
const char V_JSON_UTF8[] = "application/json;charset=UTF-8";
const char V_OCTET_STREAM[] = "application/octet-stream";
const char V_IMAGE_JPEG[] = "image/jpeg";
const char V_IMAGE_PNG[] = "image/png";
const char V_IMAGE_GIF[] = "image/gif";
const char V_IMAGE_BMP[] = "image/bmp";
const char V_XHR[] = "XMLHttpRequest";
const char V_KEEP_ALIVE[] = "keep-alive";
const char C_FORM_NONE = 0;
const char V_FORM_URLENCODED[] = "application/x-www-form-urlencoded";
const char C_FORM_URLENCODED = 1;
const char V_FORM_DATA[] = "multipart/form-data";
const char C_FORM_DATA = 2;

const int HTTP_200 = 200;
const int HTTP_301 = 301;
const int HTTP_302 = 302;
const int HTTP_304 = 304;
const int HTTP_400 = 400;
const int HTTP_401 = 401;
const int HTTP_403 = 403;
const int HTTP_404 = 404;
const int HTTP_405 = 405;
const int HTTP_406 = 406;
const int HTTP_500 = 500;
const char HTTP_STATUS_200[] = "HTTP/1.1 200 OK\n";
const char HTTP_STATUS_301[] = "HTTP/1.1 301 Moved Permanently\n";
const char HTTP_STATUS_302[] = "HTTP/1.1 302 Moved Temporarily\n";
const char HTTP_STATUS_304[] = "HTTP/1.1 304 Not Modified\n";
const char HTTP_STATUS_400[] = "HTTP/1.1 400 Bad Request\n";
const char HTTP_STATUS_403[] = "HTTP/1.1 403 Forbidden\n";
const char HTTP_STATUS_404[] = "HTTP/1.1 404 Not Found\n";
const char HTTP_STATUS_405[] = "HTTP/1.1 405 Method Not Allowed\n";
const char HTTP_STATUS_406[] = "HTTP/1.1 406 Not Acceptable\n";
const char HTTP_STATUS_500[] = "HTTP/1.1 500 Internal Server Error\n";

const char DELIM_ROW = '\1';
const char DELIM_FIELD = '\2';


} /* namespace shot */

#endif /* end of include guard: SHOT_HTTP_H */
