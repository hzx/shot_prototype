#include "gmock/gmock.h"
#include "Request.h"
#include "http.h"
using namespace testing;


class RequestTest: public testing::Test {
public:
  shot::Request request;
};


TEST_F(RequestTest, isKeepAlive) {
  request.headers.insert({shot::KL_CONNECTION, shot::V_KEEP_ALIVE});

  ASSERT_THAT(request.isKeepAlive(), testing::Eq(true));
}


TEST_F(RequestTest, isXhr) {
  request.headers.insert({shot::KL_XHR, shot::V_XHR});

  ASSERT_THAT(request.isXhr(), testing::Eq(true));
}
