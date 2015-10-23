#include <unordered_set>
#include "gmock/gmock.h"
#include "translit.h"


TEST(TranslitTest, toString) {
  std::wstring src = L"Тест 0123";
  std::string expected = "Тест 0123";

  ASSERT_THAT(shot::toString(src), expected);
}


TEST(TranslitTest, toWstring) {
  std::string src = "Тест 0123";
  std::wstring expected = L"Тест 0123";

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
  std::wstring raw = L"Это тест - \"Название\" 0123456789";
  std::wstring raw2 = L"This test - \"Title\"";
  std::wstring expected = L"Eto test - \"Nazvanie\" 0123456789";

  ASSERT_THAT(shot::translit(raw), expected);
  ASSERT_THAT(shot::translit(raw2), raw2);
}


TEST(TranslitTest, slugify) {
  std::wstring raw = L"Это тест - \"Название\" 0123456789";
  std::wstring raw2 = L"This test - \"Title\" 0123456789";
  std::string raw3 = "Это тест - \"Название\" 0123456789";
  std::wstring expected = L"eto-test-nazvanie-0123456789";
  std::wstring expected2 = L"this-test-title-0123456789";
  std::string expected3 = "eto-test-nazvanie-0123456789";

  ASSERT_THAT(shot::slugify(raw), expected);
  ASSERT_THAT(shot::slugify(raw2), expected2);
  ASSERT_THAT(shot::slugify(raw3), expected3);
}


TEST(TranslitTest, createTags) {
  std::string name1 = " Hello, a \"World-123\" a";
  std::unordered_set<std::string> tags1 = {"hello", "world", "123"};
  
  std::unordered_set<std::string> actual1;
  shot::createTags(name1, actual1);

  ASSERT_THAT(actual1.size(), tags1.size());
  ASSERT_THAT(actual1, tags1);

  std::string name2 = " Привет, в \"Мир-123\" в";
  std::unordered_set<std::string> tags2 = {"привет", "мир", "123"};
  
  std::unordered_set<std::string> actual2;
  shot::createTags(name2, actual2);

  ASSERT_THAT(actual2.size(), tags2.size());
  ASSERT_THAT(actual2, tags2);
}


TEST(TranslitTest, createTagsWithoutPretexts) {
  std::string name1 = "дом на крыше, под крышей, из-за гор";
  std::unordered_set<std::string> tags1 = {"дом", "крыше", "крышей", "гор"};

  std::unordered_set<std::string> actual1;
  shot::createTags(name1, actual1);

  ASSERT_THAT(actual1.size(), tags1.size());
  ASSERT_THAT(actual1, tags1);
}


TEST(TranslitTest, createTagsNoDoubles) {
  std::string name = "дом и второй дом";
  std::unordered_set<std::string> tags = {"дом", "второй"};

  std::unordered_set<std::string> actual;
  shot::createTags(name, actual);

  ASSERT_THAT(actual.size(), tags.size());
  ASSERT_THAT(actual, tags);
}


TEST(TranslitTest, createSearchTags) {
  std::unordered_set<std::string> tags1 = {"привет", "мир", "123"};
  std::unordered_set<std::string> incomplete1 = {"пр", "при", "прив", "приве",
    "привет", "ми", "мир", "12", "123"};

  std::unordered_set<std::string> tags2 = {"hello", "world", "123"};
  std::unordered_set<std::string> incomplete2 = {"he", "hel", "hell", "hello",
    "wo", "wor", "worl", "world", "12", "123"};

  std::unordered_set<std::string> actual1;
  shot::createSearchTags(tags1, actual1);

  ASSERT_THAT(actual1.size(), incomplete1.size());
  ASSERT_THAT(actual1, incomplete1);

  std::unordered_set<std::string> actual2;
  shot::createSearchTags(tags2, actual2);

  ASSERT_THAT(actual2.size(), incomplete2.size());
  ASSERT_THAT(actual2, incomplete2);
}


TEST(TranslitTest, createSearchTagsRawVersion) {
  std::string text1 = " Hello, a \"World-123\" a";
  std::unordered_set<std::string> incomplete1 = {"he", "hel", "hell", "hello",
    "wo", "wor", "worl", "world", "12", "123"};

  std::unordered_set<std::string> actual1;
  shot::createSearchTags(text1, actual1);

  ASSERT_THAT(actual1.size(), incomplete1.size());
  ASSERT_THAT(actual1, incomplete1);

  std::string text2 = " Привет, в \"Мир-123\" в";
  std::unordered_set<std::string> incomplete2 = {"пр", "при", "прив", "приве",
    "привет", "ми", "мир", "12", "123"};

  std::unordered_set<std::string> actual2;
  shot::createSearchTags(text2, actual2);

  ASSERT_THAT(actual2.size(), incomplete2.size());
  ASSERT_THAT(actual2, incomplete2);
}


TEST(TranslitTest, createSearchTagsFrom2SymbolTag) {
  std::string text = "abc-01";
  std::unordered_set<std::string> tags = {"ab", "abc", "01"};

  std::unordered_set<std::string> actual;
  shot::createSearchTags(text, actual);

  ASSERT_THAT(tags.size(), actual.size());
  ASSERT_THAT(tags, actual);
}
