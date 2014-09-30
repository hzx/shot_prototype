#include "gmock/gmock.h"
#include "shot/translit.h"


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
