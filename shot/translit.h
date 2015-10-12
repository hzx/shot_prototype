#ifndef SHOT_TRANSLIT_H
#define SHOT_TRANSLIT_H


#include <string>


using std::string;
using std::wstring;


namespace shot {


string toString(wstring& ws);
wstring toWstring(string& s);
wstring replaceDashSpace(wstring& text);
wstring cleanSlug(wstring& text);
wstring translit(wstring& text);
string translit(string& text);
wstring slugify(wstring& text);
string slugify(string& text);
void createTags(std::string& text, std::vector<std::string>& tags);
void createSearchTags(std::vector<std::string>& tags,
    std::vector<std::string>& searchTags);
void createIncompleteTags(std::string& word, std::vector<std::string>& tags);

} /* namespace shot */


#endif /* end of include guard: SHOT_TRANSLIT_H */
