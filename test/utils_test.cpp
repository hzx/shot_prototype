#include "gmock/gmock.h"
#include "utils.h"


using std::string;
using std::vector;


TEST(ShotUtils, joinUrlSlugs) {
  vector<string> address = {"static", "app", "client.js"};

  string actual = shot::joinUrlSlugs(address.begin() + 1, address.end());
  string expected = "/app/client.js";

  ASSERT_THAT(actual, expected);
}


TEST(ShotUtils, getFilename) {
  string path = "/tmp/some/image.jpg";

  string actual = shot::getFilename(path);
  string expected = "image.jpg";

  ASSERT_THAT(actual, expected);
}


TEST(ShotUtils, encodeUrl) {
  string params = "dem Привет";
  string encoded = "dem%20%D0%9F%D1%80%D0%B8%D0%B2%D0%B5%D1%82";

  ASSERT_THAT(shot::encodeUrl(params), encoded);
}


TEST(ShotUtils, decodeUrl) {
  string encoded = "dem%20%D0%9F%D1%80%D0%B8%D0%B2%D0%B5%D1%82";
  string decoded = "dem Привет";

  ASSERT_THAT(shot::decodeUrl(encoded), decoded);
}


TEST(ShotUtils, escape) {
  string html = "<a href=\"/send?user=test&url=admin\">go</a>";
  string expected = "&lta href=&quot/send?user=test&ampurl=admin&quot&gtgo&lt/a&gt";

  ASSERT_THAT(shot::escape(html), expected);
}


TEST(ShotUtils, base64Encode) {
  string source = "Test тест 123";
  string expected = "VGVzdCDRgtC10YHRgiAxMjM=";

  ASSERT_THAT(shot::base64Encode(source), expected);
}


TEST(ShotUtils, base64Decode) {
  string source = "VGVzdCDRgtC10YHRgiAxMjM=";
  string expected = "Test тест 123";

  ASSERT_THAT(shot::base64Decode(source), expected);
}


TEST(ShotUtils, createSignature) {
  string secret = "N8uCjkq9QxmHdl/i5oyfeG4FZcO8ckSgk2jq8BeVHdg=";
  string toSign = "test value";
  /* std::string expected = "7bd1eb0f45e828c9b86f5714df8eb9954f8047c9ff31bf9f50fd16a5861defb7"; */
  string expected = "c219c091cbf1ec7831e96ef8c3adafdabdad146b54ad930526d041fdfe541672";
  string actual = shot::createSignature(secret, toSign);

  ASSERT_THAT(actual, expected);
  ASSERT_THAT(actual.length(), 64);
}


TEST(ShotUtils, createSignedValue) {
}


TEST(ShotUtils, encodeCookie) {
  string secret = "/20kVLglQLCQimcBb+q2w5FhdqRyKkutuJo3u+uUf14=";
  time_t clock = 1409917858;
  string name = "passowrd";
  string value = "password value";

  string expected = "1|1:0|10:1409917858|8:passowrd|20:cGFzc3dvcmQgdmFsdWU=|5735aaee04eeecdd54ca0e3dfbc080be1c019f94d635fe823f2657387c1e4f70";

  ASSERT_THAT(shot::encodeCookie(secret, name, value, clock), expected);
}


TEST(ShotUtils, decodeCookie) {
  string secret = "/20kVLglQLCQimcBb+q2w5FhdqRyKkutuJo3u+uUf14=";
  time_t clock = 1409917858 + 10;
  int days = 3000; // set this day long, cookie will valid
  string name = "passowrd";
  string value = "1|1:0|10:1409917858|8:passowrd|20:cGFzc3dvcmQgdmFsdWU=|5735aaee04eeecdd54ca0e3dfbc080be1c019f94d635fe823f2657387c1e4f70";

  string expected = "password value";

  ASSERT_THAT(shot::decodeCookie(secret, name, value, days, clock), expected);
}


TEST(ShotUtils, uuid4RandomLength) {
  string value = shot::uuid4();

  ASSERT_THAT(value.length(), 36);
  ASSERT_THAT(value, testing::Ne(shot::uuid4()));
}


TEST(ShotUtils, createDecodeXsrf) {
  string xsrf = "1|1410083833|c006aedeb55d2d060f812ed3c77042e5";
  string token = "c006aedeb55d2d060f812ed3c77042e5";

  string actualXsrf = shot::createXsrf();
  string actualToken = shot::decodeXsrf(xsrf);

  ASSERT_THAT(actualXsrf.length(), xsrf.length());
  ASSERT_THAT(actualToken, token);
}


TEST(ShotUtils, createPassword) {
  string password = shot::createPassword();

  ASSERT_THAT(password.length(), 96);
  ASSERT_THAT(password, testing::Ne(shot::createPassword()));
}


TEST(ShotUtils, encodeSaltValue) {
  size_t length = 32;
  string raw = shot::urandom(length);
  string salt = shot::createSalt();
  string salt2 = shot::createSalt();

  ASSERT_THAT(salt, testing::Ne(salt2));
  ASSERT_THAT(shot::encodeSaltValue(salt, raw),
      testing::Ne(shot::encodeSaltValue(salt2, raw)));
}


TEST(ShotUtils, encodePassword) {
  string pass = "testpass";
  string pass2 = "ldjflafj";

  // the same password must be not equal after encode, because salt generated
  ASSERT_THAT(shot::encodePassword(pass),
      testing::Ne(shot::encodePassword(pass)));
  ASSERT_THAT(shot::encodePassword(pass),
      testing::Ne(shot::encodePassword(pass2)));
}


TEST(ShotUtils, checkPassword) {
  string pass1 = "testpass1";
  string pass2 = "testpass2";
  string pass1enc = shot::encodePassword(pass1);
  string pass2enc = shot::encodePassword(pass2);

  ASSERT_THAT(shot::checkPassword(pass1, pass1enc), true);
  ASSERT_THAT(shot::checkPassword(pass2, pass2enc), true);
  ASSERT_THAT(shot::checkPassword(pass1, pass2enc), false);
  ASSERT_THAT(shot::checkPassword(pass2, pass1enc), false);
}


TEST(ShotUtils, unquote) {
  string source = "\"Some value\"";

  ASSERT_THAT(shot::unquote(source), testing::Eq("Some value"));
}


TEST(ShotUtils, parseGeneric) {
  string source = "1:a1:2:";
  size_t size = 4;

  auto fields = shot::parseGeneric(source, ':');

  ASSERT_THAT(fields.size(), testing::Eq(size));
}


TEST(ShotUtils, split) {
  std::string text1 = "this,tags,field";
  std::vector<std::string> expected1 = {"this", "tags", "field"};

  std::vector<std::string> actual1;
  shot::split(text1, ',', actual1);

  ASSERT_THAT(actual1.size(), expected1.size());
  ASSERT_THAT(actual1, expected1);

  std::string text2 = ",empty,first,last,";
  std::vector<std::string> expected2 = {"", "empty", "first", "last", ""};

  std::vector<std::string> actual2;
  shot::split(text2, ',', actual2);

  ASSERT_THAT(actual2.size(), expected2.size());
  ASSERT_THAT(actual2, expected2);

  std::string text3 = "simple";
  std::vector<std::string> expected3 = {"simple"};

  std::vector<std::string> actual3;
  shot::split(text3, ',', actual3);

  ASSERT_THAT(actual3.size(), expected3.size());
  ASSERT_THAT(actual3, expected3);

  std::string text4 = "";
  std::vector<std::string> expected4 = {};

  std::vector<std::string> actual4;
  shot::split(text4, ',', actual4);

  ASSERT_THAT(actual4.size(), expected4.size());
  ASSERT_THAT(actual4, expected4);
}


TEST(ShotUtils, join) {
  std::vector<std::string> words1 = {"this", "tags", "field"};
  std::vector<std::string> words2 = {};
  std::vector<std::string> words3 = {"one"};

  std::string expected1 = "this,tags,field";
  std::string expected2 = "";
  std::string expected3 = "one";

  std::string actual1 = shot::join(words1, ',');
  std::string actual2 = shot::join(words2, ',');
  std::string actual3 = shot::join(words3, ',');

  ASSERT_THAT(actual1, expected1);
  ASSERT_THAT(actual2, expected2);
  ASSERT_THAT(actual3, expected3);
}


TEST(ShotUtils, move) {
  std::vector<std::string> ids1 = {"a", "b", "c", "d"};
  std::vector<std::string> expected1 = {"b", "a", "c", "d"};

  shot::move("a", "c", ids1);

  ASSERT_THAT(ids1, expected1);

  std::vector<std::string> ids2 = {"a", "b", "c", "d"};
  std::vector<std::string> expected2 = {"a", "c", "d", "b"};

  shot::move("b", "", ids2);

  ASSERT_THAT(ids2, expected2);
}


TEST(ShotUtils, insert) {
  std::vector<std::string> ids1 = {"a", "b", "c", "d"};
  std::vector<std::string> expected1 = {"a", "b", "e", "c", "d"};

  shot::insert("e", "c", ids1);

  ASSERT_THAT(ids1, expected1);

  std::vector<std::string> ids2 = {"a", "b", "c", "d"};
  std::vector<std::string> expected2 = {"a", "b", "c", "d", "e"};

  shot::insert("e", "", ids2);

  ASSERT_THAT(ids2, expected2);

  std::vector<std::string> ids3 = {"a"};
  std::vector<std::string> expected3 = {"a", "b"};

  shot::insert("b", "", ids3);

  ASSERT_THAT(ids3, expected3);
}

TEST(ShotUtils, remove) {
  std::vector<std::string> ids1 = {"a", "b", "c", "d"};
  std::vector<std::string> expected1 = {"a", "c", "d"};

  shot::remove("b", ids1);

  ASSERT_THAT(ids1, expected1);
}
