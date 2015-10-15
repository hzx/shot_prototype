#include <unordered_map>
#include <unordered_set>
#include <sstream>
#include <cwctype>
#include <locale>
#include <algorithm>
#include <stdio.h>
#include "utils.h"
#include "translit.h"


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


std::unordered_set<wchar_t> tagSymbols = {
  // upper
  // three-symbols replacements
  L'Щ',
  // on russian->english translation only first replacement will be done
  // i.e. Sch
  // but on english->russian translation both variants (Sch and SCH) will play
  /* {'Щ', "SCH"}, */
  // two-symbol replacements
  L'Ё',
  /* {'Ё', "YO"}, */
  L'Ж',
  /* {'Ж', "ZH"}, */
  L'Ц',
  /* {'Ц', "TS"}, */
  L'Ч',
  /* {'Ч', "CH"}, */
  L'Ш',
  /* {'Ш', "SH"}, */
  L'Ы',
  /* {'Ы', "YI"}, */
  L'Ю',
  /* {'Ю', "YU"}, */
  L'Я',
  /* {'Я', "YA"}, */
  // one-symbol replacements
  L'А',
  L'Б',
  L'В',
  L'Г',
  L'Д',
  L'Е',
  L'З',
  L'И',
  L'Й',
  L'К',
  L'Л',
  L'М',
  L'Н',
  L'О',
  L'П',
  L'Р',
  L'С',
  L'Т',
  L'У',
  L'Ф',
  L'Х',
  L'Э',
  L'Ъ',
  L'Ь',
  // lower
  // three-symbols replacements
  L'щ',
  // two-symbols replacements
  L'ё',
  L'ж',
  L'ц',
  L'ч',
  L'ш',
  L'ы',
  L'ю',
  L'я',
  // one-symbol replacements
  L'а',
  L'б',
  L'в',
  L'г',
  L'д',
  L'е',
  L'з',
  L'и',
  L'й',
  L'к',
  L'л',
  L'м',
  L'н',
  L'о',
  L'п',
  L'р',
  L'с',
  L'т',
  L'у',
  L'ф',
  L'х',
  L'э',
  L'ъ',
  L'ь',
  // Make english alphabet full: append english-english pairs
  // for symbols which is not used in russian-english
  // translations. Used in slugify.
  L'c',
  L'q',
  L'y',
  L'x',
  L'w',
  L'1',
  L'2',
  L'3',
  L'4',
  L'5',
  L'6',
  L'7',
  L'8',
  L'9',
  L'0',
  L'A',
  L'B',
  L'C',
  L'D',
  L'E',
  L'F',
  L'G',
  L'H',
  L'I',
  L'J',
  L'K',
  L'L',
  L'M',
  L'N',
  L'O',
  L'P',
  L'Q',
  L'R',
  L'S',
  L'T',
  L'U',
  L'V',
  L'W',
  L'X',
  L'Y',
  L'Z',

  L'a',
  L'b',
  L'c',
  L'd',
  L'e',
  L'f',
  L'g',
  L'h',
  L'i',
  L'j',
  L'k',
  L'l',
  L'm',
  L'n',
  L'o',
  L'p',
  L'q',
  L'r',
  L's',
  L't',
  L'u',
  L'v',
  L'w',
  L'x',
  L'y',
  L'z',
}; // tagSymbols


std::unordered_map<wchar_t, wchar_t> transtableToWLower = {
  {L'Щ', L'щ'},
  {L'Ё', L'ё'},
  {L'Ж', L'ж'},
  {L'Ц', L'ц'},
  {L'Ч', L'ч'},
  {L'Ш', L'ш'},
  {L'Ы', L'ы'},
  {L'Ю', L'ю'},
  {L'Я', L'я'},
  {L'А', L'а'},
  {L'Б', L'б'},
  {L'В', L'в'},
  {L'Г', L'г'},
  {L'Д', L'д'},
  {L'Е', L'е'},
  {L'З', L'з'},
  {L'И', L'и'},
  {L'Й', L'й'},
  {L'К', L'к'},
  {L'Л', L'л'},
  {L'М', L'м'},
  {L'Н', L'н'},
  {L'О', L'о'},
  {L'П', L'п'},
  {L'Р', L'р'},
  {L'С', L'с'},
  {L'Т', L'т'},
  {L'У', L'у'},
  {L'Ф', L'ф'},
  {L'Х', L'х'},
  {L'Э', L'э'},
  {L'Ъ', L'ъ'},
  {L'Ь', L'ь'},

  {L'A', L'a'},
  {L'B', L'b'},
  {L'C', L'c'},
  {L'D', L'd'},
  {L'E', L'e'},
  {L'F', L'f'},
  {L'G', L'g'},
  {L'H', L'h'},
  {L'I', L'i'},
  {L'J', L'j'},
  {L'K', L'k'},
  {L'L', L'l'},
  {L'M', L'm'},
  {L'N', L'n'},
  {L'O', L'o'},
  {L'P', L'p'},
  {L'Q', L'q'},
  {L'R', L'r'},
  {L'S', L's'},
  {L'T', L't'},
  {L'U', L'u'},
  {L'V', L'v'},
  {L'W', L'w'},
  {L'X', L'x'},
  {L'Y', L'y'},
  {L'Z', L'z'},
}; // transtableToWLower


std::unordered_map<wchar_t, wchar_t> transtableToWUpper = {
  {L'щ', L'Щ'},
  {L'ё', L'Ё'},
  {L'ж', L'Ж'},
  {L'ц', L'Ц'},
  {L'ч', L'Ч'},
  {L'ш', L'Ш'},
  {L'ы', L'Ы'},
  {L'ю', L'Ю'},
  {L'я', L'Я'},
  {L'а', L'А'},
  {L'б', L'Б'},
  {L'в', L'В'},
  {L'г', L'Г'},
  {L'д', L'Д'},
  {L'е', L'Е'},
  {L'з', L'З'},
  {L'и', L'И'},
  {L'й', L'Й'},
  {L'к', L'К'},
  {L'л', L'Л'},
  {L'м', L'М'},
  {L'н', L'Н'},
  {L'о', L'О'},
  {L'п', L'П'},
  {L'р', L'Р'},
  {L'с', L'С'},
  {L'т', L'Т'},
  {L'у', L'У'},
  {L'ф', L'Ф'},
  {L'х', L'Х'},
  {L'э', L'Э'},
  {L'ъ', L'Ъ'},
  {L'ь', L'Ь'},

  {L'a', L'A'},
  {L'b', L'B'},
  {L'c', L'C'},
  {L'd', L'D'},
  {L'e', L'E'},
  {L'f', L'F'},
  {L'g', L'G'},
  {L'h', L'H'},
  {L'i', L'I'},
  {L'j', L'J'},
  {L'k', L'K'},
  {L'l', L'L'},
  {L'm', L'M'},
  {L'n', L'N'},
  {L'o', L'O'},
  {L'p', L'P'},
  {L'q', L'Q'},
  {L'r', L'R'},
  {L's', L'S'},
  {L't', L'T'},
  {L'u', L'U'},
  {L'v', L'V'},
  {L'w', L'W'},
  {L'x', L'X'},
  {L'y', L'Y'},
  {L'z', L'Z'},
}; // transtableToWUpper

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


std::wstring toLower(std::wstring& s) {
  std::wstring lower;
  lower.resize(s.length());

  for (size_t i = 0, length = s.length(); i < length; ++i) {
    auto it = transtableToWLower.find(s[i]);
    if (it != transtableToWLower.end()) {
      lower[i] = it->second;
    } else {
      lower[i] = s[i];
    }
  }

  return lower;
}


std::wstring toUpper(std::wstring& s) {
  std::wstring lower;
  lower.resize(s.length());

  for (size_t i = 0, length = s.length(); i < length; ++i) {
    auto it = transtableToWUpper.find(s[i]);
    if (it != transtableToWUpper.end()) {
      lower[i] = it->second;
    } else {
      lower[i] = s[i];
    }
  }

  return lower;
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


void isTagSymbol(wchar_t symbol) {
}


// TODO: find simple words and filter tags
void createTags(std::string& text, std::vector<std::string>& tags) {
  std::wstring wtextMixed = toWstring(text);
  std::wstring wtext = toLower(wtextMixed);

  size_t left = 0;
  bool isLeftFound = false;
  
  for (size_t i = 0; i < wtext.length(); ++i) {
    auto it = tagSymbols.find(wtext[i]);
    if (it != tagSymbols.end()) { // tag symbol
      if (isLeftFound == false) {
        isLeftFound = true;
        left = i;
      }
    } else { // not tag symbol
      if (isLeftFound) { // found tag
        isLeftFound = false;

        if (i - left <= 1) continue; // dont add one symbol word

        std::wstring wtag = wtext.substr(left, i - left);
        auto tag = toString(wtag);
        tags.push_back(tag);
      }
    }
  }

  // add last tag
  if (isLeftFound) {
    if (wtext.size() - left >= 2) { // dont add one symbol word

      std::wstring wtag = wtext.substr(left);
      auto tag = toString(wtag);
      tags.push_back(tag);
    }
  }
}


void createSearchTags(std::vector<std::string>& tags,
    std::vector<std::string>& searchTags) {
  for (std::string& tag: tags) {
    createIncompleteTags(tag, searchTags);
  }
}


void createSearchTags(std::string& text, std::vector<std::string>& searchTags) {
  std::vector<std::string> tags;

  createTags(text, tags);
  createSearchTags(tags, searchTags);
}


void createIncompleteTags(std::string& word, std::vector<std::string>& tags) {
  std::wstring wword = toWstring(word);
  if (wword.length() <= 2) return;

  for (size_t i = 2; i < wword.length(); ++i) {
    std::wstring wtag = wword.substr(0, i);
    std::string tag = toString(wtag);
    tags.push_back(tag);
  }

  tags.push_back(word);
}


} /* namespace shot */
