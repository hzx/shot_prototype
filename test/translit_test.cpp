#include "gmock/gmock.h"
#include "translit.h"


TEST(TranslitTest, toString) {
  wstring src = L"Тест 0123";
  string expected = "Тест 0123";

  ASSERT_THAT(shot::toString(src), expected);
}


TEST(TranslitTest, toWstring) {
  string src = "Тест 0123";
  wstring expected = L"Тест 0123";

  ASSERT_THAT(shot::toWstring(src), expected);
}


TEST(TranslitTest, toLower) {
  std::wstring text =
    L"ЩЁЖЦЧШЫЮЯАБВГДЕЗИЙКЛМНОПРСТУФХЭЪЬABCDEFGHIJKLMNOPQRSTUVWXYZ";
  std::wstring expected =
    L"щёжцчшыюяабвгдезийклмнопрстуфхэъьabcdefghijklmnopqrstuvwxyz";

  std::wstring actual = shot::toLower(text);

  ASSERT_THAT(actual, expected);
}


TEST(TranslitTest, toUpper) {
  std::wstring text =
    L"щёжцчшыюяабвгдезийклмнопрстуфхэъьabcdefghijklmnopqrstuvwxyz";
  std::wstring expected =
    L"ЩЁЖЦЧШЫЮЯАБВГДЕЗИЙКЛМНОПРСТУФХЭЪЬABCDEFGHIJKLMNOPQRSTUVWXYZ";

  std::wstring actual = shot::toUpper(text);

  ASSERT_THAT(actual, expected);
}


TEST(TranslitTest, translit) {
  wstring raw = L"Это тест - \"Название\" 0123456789";
  wstring raw2 = L"This test - \"Title\"";
  wstring expected = L"Eto test - \"Nazvanie\" 0123456789";

  ASSERT_THAT(shot::translit(raw), expected);
  ASSERT_THAT(shot::translit(raw2), raw2);
}


TEST(TranslitTest, slugify) {
  wstring raw = L"Это тест - \"Название\" 0123456789";
  wstring raw2 = L"This test - \"Title\" 0123456789";
  string raw3 = "Это тест - \"Название\" 0123456789";
  wstring expected = L"eto-test-nazvanie-0123456789";
  wstring expected2 = L"this-test-title-0123456789";
  string expected3 = "eto-test-nazvanie-0123456789";

  ASSERT_THAT(shot::slugify(raw), expected);
  ASSERT_THAT(shot::slugify(raw2), expected2);
  ASSERT_THAT(shot::slugify(raw3), expected3);
}


TEST(TranslitTest, createTags) {
  std::string name1 = " Hello, a \"World-123\" a";
  std::vector<std::string> tags1 = {"hello", "world", "123"};
  
  std::vector<std::string> actual1;
  shot::createTags(name1, actual1);

  ASSERT_THAT(actual1.size(), tags1.size());
  ASSERT_THAT(actual1, tags1);

  std::string name2 = " Привет, в \"Мир-123\" в";
  std::vector<std::string> tags2 = {"привет", "мир", "123"};
  
  std::vector<std::string> actual2;
  shot::createTags(name2, actual2);

  ASSERT_THAT(actual2.size(), tags2.size());
  ASSERT_THAT(actual2, tags2);
}


TEST(TranslitTest, createSearchTags) {
  std::vector<std::string> tags1 = {"привет", "мир", "123"};
  std::vector<std::string> incomplete1 = {"пр", "при", "прив", "приве",
    "привет", "ми", "мир", "12", "123"};

  std::vector<std::string> tags2 = {"hello", "world", "123"};
  std::vector<std::string> incomplete2 = {"he", "hel", "hell", "hello",
    "wo", "wor", "worl", "world", "12", "123"};

  std::vector<std::string> actual1;
  shot::createSearchTags(tags1, actual1);

  ASSERT_THAT(actual1.size(), incomplete1.size());
  ASSERT_THAT(actual1, incomplete1);

  std::vector<std::string> actual2;
  shot::createSearchTags(tags2, actual2);

  ASSERT_THAT(actual2.size(), incomplete2.size());
  ASSERT_THAT(actual2, incomplete2);
}
