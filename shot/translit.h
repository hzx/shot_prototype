#pragma once
#include <string>
#include <unordered_set>


namespace shot {


std::string toString(std::wstring const& ws);
std::wstring toWstring(std::string const& s);
std::wstring toLower(std::wstring const& s);
std::wstring toUpper(std::wstring const& s);
std::wstring replaceDashSpace(std::wstring const& text);
std::wstring cleanSlug(std::wstring const& text);
std::wstring translit(std::wstring const& text);
std::string translit(std::string const& text);
std::wstring slugify(std::wstring const& text);
std::string slugify(std::string const& text);
void createTags(std::string const& text, std::unordered_set<std::string>& tags);
void createSearchTags(std::unordered_set<std::string> const& tags,
    std::unordered_set<std::string>& searchTags);
void createSearchTags(std::string const& text,
    std::unordered_set<std::string>& searchTags);
void createIncompleteTags(std::string const& word,
    std::unordered_set<std::string>& tags);

} /* namespace shot */
