#include <string>
#include "Handler.h"
#include "gmock/gmock.h"


using std::string;


class HandlerTest: public testing::Test {
public:
  shot::Handler handler;
};


TEST_F(HandlerTest, getXsrfPersistent) {
  /* ASSERT_THAT(handler.getXsrf(), testing::Eq(handler.getXsrf())); */
}
