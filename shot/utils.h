#ifndef SHOT_UTILS_H
#define SHOT_UTILS_H


#include <string>
#include <vector>
#include <ctime>


using std::string;
using std::vector;


namespace shot {


string getExtHeader(const string& ext);
bool pathExists(const char* filename);
string pathLoad(const char* filename);
string pathMd5(const char* filename);
void toCamelFormat(string& text);
void toLower(string& text);
string joinUrlSlugs(vector<string>::iterator begin, vector<string>::iterator end);
string getDirectory();
void makeDir(const string& path);
string getExtension(const string& filename);
string randomFilename(const string& filename);
void copyFile(const string& dest, const string& src);
void tolower(string& value);
string encodeUrl(const string& value);
string decodeUrl(const string& value);
string escape(const string& value);
string escapeJsonString(const string& value);
/* string base64Encode(const char* value); */
string base64_decode(string const& encoded_string);
string createSignature(const string& secret, const string& toSign);

string createSignedValue(const string& secret,
    const string& name, const string& value, time_t clock);

string consumeField(string::iterator begin, string::iterator end);

string decodeSignedValue(const string& secret,
    const string& name, const string& value,
    int maxAgeDays, time_t clock);

string base64Encode(const string& value);
/* string base64Decode(const char* value); */
string base64Decode(const string& value);
string encodeCookie(const string& secret,
    const string& name, const string& value, time_t clock);
string decodeCookie(const string& secret,
    const string& name, const string& value,
    int maxAgeDays, time_t clock);

string urandom(size_t n);
string uuid4();
string createXsrf();
string decodeXsrf(const string& value);

string createPassword();
string encodeSaltValue(const string& salt, const string& value);
string createSalt();
string encodePassword(const string& raw);
bool checkPassword(const string& raw, const string& encoded);
string unquote(string& source);
vector<string> parseGeneric(string const& src, const char delim);
vector<string> parseRows(string const& src);
vector<string> parseFields(string const& src);

  
} /* namespace shot */

#endif /* end of include guard: SHOT_UTILS_H */
