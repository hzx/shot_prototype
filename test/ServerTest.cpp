#include "gmock/gmock.h"
#include "Server.h"
using namespace testing;


class ServerTest: public testing::Test {
public:
  shot::Options options;
  shot::Logger logger;
  shot::Server server;

  ServerTest();
};


ServerTest::ServerTest()
    : server() {
}


/* TEST_F(ServerTest, ServerRun) { */
/*   ASSERT_THAT(server.run(), Eq(0)); */
/* } */


/* TEST_F(ServerTest, ServerStop) { */
/*   ASSERT_THAT(server.stop(), Eq(0)); */
/* } */


TEST_F(ServerTest, Connection) {
  /* server.listen(); */
  /* ASSERT_THAT(server. */
}
