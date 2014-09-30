#include <unordered_map>
#include <sstream>
#include <cwctype>
#include <locale>
#include "shot/utils.h"
#include "shot/translit.h"


using std::wstring;


namespace shot {


/* typedef std::wstring_convert<std::codecvt<wchar_t>> type_convert; */
typedef std::codecvt<wchar_t, char, std::mbstate_t> type_codec;

std::locale locale("en_US.UTF-8");
const type_codec& codec = std::use_facet<type_codec>(locale);


std::unordered_map<wchar_t, wstring> transtable = {
  {L'\'', L"'"},
  {L'"', L"\""},
  {L'‘', L"'"},
  {L'’', L"'"},
  {L'«', L"\""},
  {L'»', L"\""},
  {L'“', L"\""},
  {L'”', L"\""},
  {L'–', L"-"},  // en dash
  {L'—', L"-"},  // em dash
  {L'‒', L"-"},  // figure dash
  {L'−', L"-"},  // minus
  {L'-', L"-"},  // dash
  {L'…', L"..."},
  {L'№', L"#"},
  // upper
  // three-symbols replacements
  {L'Щ', L"Sch"},
  // on russian->english translation only first replacement will be done
  // i.e. Sch
  // but on english->russian translation both variants (Sch and SCH) will play
  /* {'Щ', "SCH"}, */
  // two-symbol replacements
  {L'Ё', L"Yo"},
  /* {'Ё', "YO"}, */
  {L'Ж', L"Zh"},
  /* {'Ж', "ZH"}, */
  {L'Ц', L"Ts"},
  /* {'Ц', "TS"}, */
  {L'Ч', L"Ch"},
  /* {'Ч', "CH"}, */
  {L'Ш', L"Sh"},
  /* {'Ш', "SH"}, */
  {L'Ы', L"Yi"},
  /* {'Ы', "YI"}, */
  {L'Ю', L"Yu"},
  /* {'Ю', "YU"}, */
  {L'Я', L"Ya"},
  /* {'Я', "YA"}, */
  // one-symbol replacements
  {L'А', L"A"},
  {L'Б', L"B"},
  {L'В', L"V"},
  {L'Г', L"G"},
  {L'Д', L"D"},
  {L'Е', L"E"},
  {L'З', L"Z"},
  {L'И', L"I"},
  {L'Й', L"J"},
  {L'К', L"K"},
  {L'Л', L"L"},
  {L'М', L"M"},
  {L'Н', L"N"},
  {L'О', L"O"},
  {L'П', L"P"},
  {L'Р', L"R"},
  {L'С', L"S"},
  {L'Т', L"T"},
  {L'У', L"U"},
  {L'Ф', L"F"},
  {L'Х', L"H"},
  {L'Э', L"E"},
  {L'Ъ', L"`"},
  {L'Ь', L"'"},
  // lower
  // three-symbols replacements
  {L'щ', L"sch"},
  // two-symbols replacements
  {L'ё', L"yo"},
  {L'ж', L"zh"},
  {L'ц', L"ts"},
  {L'ч', L"ch"},
  {L'ш', L"sh"},
  {L'ы', L"yi"},
  {L'ю', L"yu"},
  {L'я', L"ya"},
  // one-symbol replacements
  {L'а', L"a"},
  {L'б', L"b"},
  {L'в', L"v"},
  {L'г', L"g"},
  {L'д', L"d"},
  {L'е', L"e"},
  {L'з', L"z"},
  {L'и', L"i"},
  {L'й', L"j"},
  {L'к', L"k"},
  {L'л', L"l"},
  {L'м', L"m"},
  {L'н', L"n"},
  {L'о', L"o"},
  {L'п', L"p"},
  {L'р', L"r"},
  {L'с', L"s"},
  {L'т', L"t"},
  {L'у', L"u"},
  {L'ф', L"f"},
  {L'х', L"h"},
  {L'э', L"e"},
  {L'ъ', L"`"},
  {L'ь', L"'"},
  // Make english alphabet full: append english-english pairs
  // for symbols which is not used in russian-english
  // translations. Used in slugify.
  {L'c', L"c"},
  {L'q', L"q"},
  {L'y', L"y"},
  {L'x', L"x"},
  {L'w', L"w"},
  {L'1', L"1"},
  {L'2', L"2"},
  {L'3', L"3"},
  {L'4', L"4"},
  {L'5', L"5"},
  {L'6', L"6"},
  {L'7', L"7"},
  {L'8', L"8"},
  {L'9', L"9"},
  {L'0', L"0"},
  {L'A', L"A"},
  {L'B', L"B"},
  {L'C', L"C"},
  {L'D', L"D"},
  {L'E', L"E"},
  {L'F', L"F"},
  {L'G', L"G"},
  {L'H', L"H"},
  {L'I', L"I"},
  {L'J', L"J"},
  {L'K', L"K"},
  {L'L', L"L"},
  {L'M', L"M"},
  {L'N', L"N"},
  {L'O', L"O"},
  {L'P', L"P"},
  {L'Q', L"Q"},
  {L'R', L"R"},
  {L'S', L"S"},
  {L'T', L"T"},
  {L'U', L"U"},
  {L'V', L"V"},
  {L'W', L"W"},
  {L'X', L"X"},
  {L'Y', L"Y"},
  {L'Z', L"Z"},

  {L'a', L"a"},
  {L'b', L"b"},
  {L'c', L"c"},
  {L'd', L"d"},
  {L'e', L"e"},
  {L'f', L"f"},
  {L'g', L"g"},
  {L'h', L"h"},
  {L'i', L"i"},
  {L'j', L"j"},
  {L'k', L"k"},
  {L'l', L"l"},
  {L'm', L"m"},
  {L'n', L"n"},
  {L'o', L"o"},
  {L'p', L"p"},
  {L'q', L"q"},
  {L'r', L"r"},
  {L's', L"s"},
  {L't', L"t"},
  {L'u', L"u"},
  {L'v', L"v"},
  {L'w', L"w"},
  {L'x', L"x"},
  {L'y', L"y"},
  {L'z', L"z"},

  {L' ', L" "},
}; // transtable


string toString(wstring& ws) {
  /* const type_codec& codec = std::use_facet<type_codec>(locale); */
  // fill objects
  size_t size = ws.length() * sizeof(wchar_t);
  char buf[size];
  std::mbstate_t state = std::mbstate_t();
  const wchar_t* pw;
  char* pc;

  auto result = codec.out(state, &ws[0], &ws[ws.length()], pw,
    &buf[0], &buf[size], pc);

  if (result != type_codec::ok) {
    return "not ok";
  }

  string s(&buf[0], pc);

  return s;
}


wstring toWstring(string& s) {
  /* const type_codec& codec = std::use_facet<type_codec>(locale); */
  // fill objects
  size_t length = s.length();
  wchar_t buf[length];
  std::mbstate_t state = std::mbstate_t();
  const char* pc;
  wchar_t* pw;

  auto result = codec.in(state, &s[0], &s[length], pc,
    &buf[0], &buf[length], pw);

  if (result != type_codec::ok) {
    return L"";
  }

  wstring ws(&buf[0], pw);

  return ws;
}


wstring replaceDashSpace(wstring& text) {
  std::wostringstream buf;
  wchar_t ch;
  bool lastDash = false;
  bool lastSpace = false;

  for (int i = 0, length = text.length(); i < length; ++i) {
    ch = text[i];

    switch (ch) {
      case L' ':
        lastSpace = true;
        break;
      case L'-':
        lastSpace = false;
        lastDash = true;
        break;
      default:
        if (lastSpace or lastDash) {
          buf << L'-';
        }
        buf << ch;
        lastSpace = false;
        lastDash = false;
        break;
    } // switch
  }

  return buf.str();
}


wstring cleanSlug(wstring& text) {
  std::wostringstream buf;

  wchar_t ch;
  for (size_t i = 0, length = text.length(); i < length; ++i) {
    ch = std::towlower(text[i]);
    if (std::iswalnum(ch) or ch == L'-') buf << ch;
  }

  return buf.str();
}


wstring translit(wstring& text) {
  std::wostringstream buf;

  for (size_t i = 0, length = text.length(); i < length; ++i) {
    auto it = transtable.find(text[i]);
    if (it != transtable.end()) {
      buf << it->second;
    }
  }

  return buf.str();
}


string translit(string& text) {
  wstring wtext = toWstring(text);
  wstring wres = translit(wtext);
  return toString(wres);
}


wstring slugify(wstring& text) {
  wstring data = replaceDashSpace(text);

  wstring result = translit(data);

  return cleanSlug(result);
}


string slugify(string& text) {
  wstring wtext = toWstring(text);
  wstring wres = slugify(wtext);
  return toString(wres);
}


} /* namespace shot */
