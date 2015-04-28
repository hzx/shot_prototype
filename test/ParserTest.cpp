#include <string>
#include <vector>
#include <memory>
#include <iterator>
#include <sstream>
#include <algorithm>
#include "gmock/gmock.h"
#include "Parser.h"
#include "Options.h"
#include "http.h"

string sourceGet = R"(GET /d HTTP/1.1
Host: localhost:8000
Connection: keep-alive
X-Requested-With: XMLHttpRequest
Cache-Control: max-age=0
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8
User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/37.0.2062.120 Safari/537.36
Accept-Encoding: gzip,deflate,sdch
Accept-Language: en-US,en;q=0.8,ru;q=0.6
Cookie: _xsrf=1|1411080605|fbcf4f5dc799a924af609a47067ab274; user=1|1:0|10:1412243203|4:user|32:NTQyN2I1NDcyNGYwZDYwMzJjZGIzNTc4|52c3e9a7825217304a9c315707faaa7d1107117581c10a657bd845bd0cf5d548)";

string sourceUrl = "GET /d HTTP/1.1";

string sourceHeaders = R"(Host: localhost:8000
Connection: keep-alive
Cache-Control: max-age=0
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8
User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/37.0.2062.120 Safari/537.36
Accept-Encoding: gzip,deflate,sdch
Accept-Language: en-US,en;q=0.8,ru;q=0.6
Cookie: _xsrf=1|1411080605|fbcf4f5dc799a924af609a47067ab274; user=1|1:0|10:1412243203|4:user|32:NTQyN2I1NDcyNGYwZDYwMzJjZGIzNTc4|52c3e9a7825217304a9c315707faaa7d1107117581c10a657bd845bd0cf5d548)";

string sourceCookie = R"(Cookie: _xsrf=1|1411080605|fbcf4f5dc799a924af609a47067ab274; user=1|1:0|10:1412243203|4:user|32:NTQyN2I1NDcyNGYwZDYwMzJjZGIzNTc4|52c3e9a7825217304a9c315707faaa7d1107117581c10a657bd845bd0cf5d548)";

string sourceParams = R"(login=admin&password=an64imperia64)";

string sourceFileHeaders = R"(Content-Disposition: form-data; name="77e691982e1e74bf7c05a32cf593af5c.gif"; filename="77e691982e1e74bf7c05a32cf593af5c.gif"
Content-Type: image/gif)";

string sourceContentDisposition = R"(form-data; name="77e691982e1e74bf7c05a32cf593af5c.gif"; filename="77e691982e1e74bf7c05a32cf593af5c.gif)";


class ParserTest: public testing::Test {
public:
  ParserTest();
  shot::Request request;
  shot::Parser parser;
};


ParserTest::ParserTest() {
}


TEST_F(ParserTest, ParserTestParseMethodGet) {
  string source = "GET /d HTTP/1.1";

  parser.parseMethod(request, source, 3);

  ASSERT_THAT(request.method, testing::Eq(shot::C_GET));
  ASSERT_THAT(request.isBad, testing::Eq(false));
  ASSERT_THAT(request.isDone, testing::Eq(true));
}


TEST_F(ParserTest, ParserTestParseMethodPost) {
  string source = "POST /d HTTP/1.1";

  parser.parseMethod(request, source, 4);

  ASSERT_THAT(request.method, testing::Eq(shot::C_POST));
  ASSERT_THAT(request.isBad, testing::Eq(false));
  ASSERT_THAT(request.isDone, testing::Eq(false));
}


TEST_F(ParserTest, ParserTestParseMethodPut) {
  string source = "PUT /d HTTP/1.1";

  parser.parseMethod(request, source, 3);

  ASSERT_THAT(request.method, testing::Eq(shot::C_NONE));
  ASSERT_THAT(request.isBad, testing::Eq(true));
  ASSERT_THAT(request.isDone, testing::Eq(true));
}


TEST_F(ParserTest, ParserTestParseUrl) {
  string source = "/d";

  parser.parseUrl(request, source, 0, source.length());

  ASSERT_THAT(request.slugs.size(), testing::Eq(1));
  ASSERT_THAT(request.slugs[0], testing::Eq("d"));
}


TEST_F(ParserTest, ParserTestParseUrlSimple) {
  string source = "/";

  parser.parseUrl(request, source, 0, source.length());

  ASSERT_THAT(request.slugs.size(), testing::Eq(1));
  ASSERT_THAT(request.slugs[0], testing::Eq(""));
}


TEST_F(ParserTest, ParserTestParseUrl2Params) {
  string source = "/ware/item1";

  parser.parseUrl(request, source, 0, source.length());

  ASSERT_THAT(request.slugs.size(), testing::Eq(2));
  ASSERT_THAT(request.slugs[0], testing::Eq("ware"));
  ASSERT_THAT(request.slugs[1], testing::Eq("item1"));
}


TEST_F(ParserTest, ParserTestParserUrlQuery) {
  string source = "/ware/someitem?user=vasya";

  parser.parseUrl(request, source, 0, source.length());

  ASSERT_THAT(request.slugs.size(), testing::Eq(2));
  ASSERT_THAT(request.slugs[0], testing::Eq("ware"));
  ASSERT_THAT(request.slugs[1], testing::Eq("someitem"));
}


TEST_F(ParserTest, ParseTestParseUrlWithCloseSlash) {
  string source = "/hi/";

  parser.parseUrl(request, source, 0, source.length());

  ASSERT_THAT(request.slugs.size(), testing::Eq(1));
  ASSERT_THAT(request.slugs[0], testing::Eq("hi"));
}


TEST_F(ParserTest, ParseTestParseUrlWithDoubleSlash) {
  string source = "/ware//item02";

  parser.parseUrl(request, source, 0, source.length());

  ASSERT_THAT(request.slugs.size(), testing::Eq(2));
  ASSERT_THAT(request.slugs[0], testing::Eq("ware"));
  ASSERT_THAT(request.slugs[1], testing::Eq("item02"));
}


TEST_F(ParserTest, ParserTestParseVersion) {
  string source = "HTTP/1.1";

  parser.parseVersion(request, sourceGet, 0, sourceGet.length());

  // TODO: think about version usage

  /* ASSERT_THAT(request.version, testing::Eq("HTTP/1.1")); */
}


TEST_F(ParserTest, ParserTestParseHeaders) {
  parser.parseHeaders(request.headers, sourceHeaders, 0,  sourceHeaders.length());

  ASSERT_THAT(request.headers.size(), testing::Eq(8));
  ASSERT_THAT(request.headers["host"], testing::Eq("localhost:8000"));
  ASSERT_THAT(request.headers["connection"], testing::Eq("keep-alive"));
  ASSERT_THAT(request.headers["cache-control"], testing::Eq("max-age=0"));
  ASSERT_THAT(request.headers["accept"], testing::Eq("text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8"));
  ASSERT_THAT(request.headers["user-agent"], testing::Eq("Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/37.0.2062.120 Safari/537.36"));
  ASSERT_THAT(request.headers["accept-encoding"], testing::Eq("gzip,deflate,sdch"));
  ASSERT_THAT(request.headers["accept-language"], testing::Eq("en-US,en;q=0.8,ru;q=0.6"));
  ASSERT_THAT(request.headers["cookie"], testing::Eq("_xsrf=1|1411080605|fbcf4f5dc799a924af609a47067ab274; user=1|1:0|10:1412243203|4:user|32:NTQyN2I1NDcyNGYwZDYwMzJjZGIzNTc4|52c3e9a7825217304a9c315707faaa7d1107117581c10a657bd845bd0cf5d548"));
}


TEST_F(ParserTest, ParserTestParseCookie) {
  parser.parseCookie(request.cookie, sourceCookie);

  ASSERT_THAT(request.cookie.size(), testing::Eq(2));
}


TEST_F(ParserTest, ParserTestParseParams) {
  parser.parseParams(request.params, sourceParams, 0, sourceParams.length());

  ASSERT_THAT(request.params.size(), testing::Eq(2));
}


TEST_F(ParserTest, ParserTestParseContentDisposition) {
  parser.parseContentDisposition(request.file, sourceContentDisposition);

  ASSERT_THAT(request.file.name, testing::Eq("77e691982e1e74bf7c05a32cf593af5c.gif"));
  ASSERT_THAT(request.file.filename, testing::Eq("77e691982e1e74bf7c05a32cf593af5c.gif"));
}


TEST_F(ParserTest, ParserTestRequestIsXhrKeepAlive) {
  parser.parse(request, sourceGet, sourceGet.length());

  ASSERT_THAT(request.isXhr(), testing::Eq(true));
  ASSERT_THAT(request.isKeepAlive(), testing::Eq(true));
}
