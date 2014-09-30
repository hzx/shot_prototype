#include <string>
#include <vector>
#include <memory>
#include <iterator>
#include <sstream>
#include <algorithm>
#include "gmock/gmock.h"
#include "shot/Parser.h"
#include "shot/Config.h"


class ParserTest: public testing::Test {
public:
  ParserTest();
  shot::Parser parser;
};


ParserTest::ParserTest() {
}


TEST_F(ParserTest, ParserTestEmptyBuffer) {
  // by default buffer empty
  /* shot::Request request; */
  /* parser.initRequest(request, buffer); */

  /* ASSERT_THAT(request.url.length(), testing::Eq(0)); */
}


TEST_F(ParserTest, ParserTestUrlWithParams) {
  /* std::string buf = "GET /contact?param1=123 HTTP/1.1\n"; */

  /* // copy request to buffer */
  /* memcpy(buffer.chunks[0], buf.data(), buf.length()); */
  /* buffer.length = buf.length(); */

  /* shot::Request request; */
  /* parser.initRequest(request, buffer); */

  /* ASSERT_THAT(request.url, testing::Eq("/contact?param1=123")); */
  /* ASSERT_THAT(request.slugs.size(), testing::Eq(1)); */
  /* ASSERT_THAT(request.slugs[0], testing::Eq("contact")); */
}


TEST_F(ParserTest, ParserTestUrlWith2Params) {
  /* std::string buf = "GET /ware/item-01ab HTTP/1.1\n"; */

  /* // copy request to buffer */
  /* memcpy(buffer.chunks[0], buf.data(), buf.length()); */
  /* buffer.length = buf.length(); */

  /* shot::Request request; */
  /* parser.initRequest(request, buffer); */

  /* ASSERT_THAT(request.url, testing::Eq("/ware/item-01ab")); */
  /* ASSERT_THAT(request.slugs.size(), testing::Eq(2)); */
  /* ASSERT_THAT(request.slugs[0], testing::Eq("ware")); */
  /* ASSERT_THAT(request.slugs[1], testing::Eq("item-01ab")); */
}
