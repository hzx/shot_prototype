#include <algorithm>
#include <iterator>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <cctype>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctime>
#include <unordered_map>
#include <openssl/sha.h>
#include <openssl/md5.h>
#include "http.h"
#include "utils.h"


using std::string;
using std::vector;
using std::ostringstream;


namespace shot {


const size_t BUF_SIZE = 4096;
const size_t FILE_BUF_SIZE = 64 * 1024;


std::unordered_map<string, string> extHeaders = {
  {"txt", V_TEXT_UTF8},
  {"html", V_HTML_UTF8},
  {"css", V_CSS_UTF8},
  {"js", V_JS_UTF8},
  {"json", V_JSON_UTF8},
  {"jpg", V_IMAGE_JPEG},
  {"jpeg", V_IMAGE_JPEG},
  {"png", V_IMAGE_PNG},
  {"gif", V_IMAGE_GIF},
  {"bmp", V_IMAGE_BMP},
};


string getExtHeader(const string& ext) {
  auto it = extHeaders.find(ext);
  return it == extHeaders.end() ? "" : it->second;
}


bool pathExists(const char* filename) {
  std::ifstream inp(filename);
  return inp.good();
}


string pathLoad(const char* filename) {
  ostringstream buf;

  std::ifstream inp(filename);
  buf << inp.rdbuf();

  return buf.str();
}


string pathMd5(const char* filename) {
  MD5_CTX context;
  char buf[FILE_BUF_SIZE];
  unsigned char hash[MD5_DIGEST_LENGTH];
  size_t count = 0;

  // open file and check is exists
  std::ifstream inp(filename);
  if (!inp.good()) return "";

  // read file by chunks and update md5hash
  MD5_Init(&context);
  /* while (!inp.eof()) { */
  for (;;) {
    count = inp.readsome(buf, FILE_BUF_SIZE);
    MD5_Update(&context, buf, count);
    if (count < FILE_BUF_SIZE) break;
  }
  MD5_Final(hash, &context);

  // copy hash to hex hash
  ostringstream hex;
  hex << std::hex << std::setfill('0');
  for (int i = 0; i < MD5_DIGEST_LENGTH; ++i) {
    hex << std::setw(2) << int(hash[i]);
  }

  return hex.str();

  ////

  /* string content = pathLoad(filename); */

  /* // get hash from file content */
  /* unsigned char result[MD5_DIGEST_LENGTH]; */
  /* MD5((unsigned char*)content.c_str(), content.length(), result); */

  /* // copy hash result to string */
  /* ostringstream buf; */

  /* buf << std::hex << std::setfill('0'); */
  /* for (int i = 0; i < MD5_DIGEST_LENGTH; ++i) { */
  /*   buf << std::setw(2) << int(result[i]); */
  /* } */

  /* return buf.str(); */
}


void toCamelFormat(string& text) {
  bool prevNotAlpha = true;
  char ch;
  for (int i = 0, length = text.length(); i < length; ++i) {
    ch = text[i];
    if (std::isalpha(ch)) {
      text[i] = prevNotAlpha ? std::toupper(ch) : std::tolower(ch);
      prevNotAlpha = false;
    } else {
      prevNotAlpha = true;
    }
  }
}


void toLower(string& text) {
  for (int i = 0, length = text.length(); i < length; ++i) {
    text[i] = std::tolower(text[i]);
  }
}


string formatField(string& value) {
  ostringstream buf;
  
  buf << value.length() << ':' << value;

  return buf.str();
}


string joinUrlSlugs(vector<string>::iterator begin, vector<string>::iterator end) {
  ostringstream buf;

  std::for_each(begin, end, [&buf](string& slug) {
    buf << '/' << slug;
  });

  return buf.str();
}


string getDirectory() {
  char buf[BUF_SIZE];
  char* path = getcwd(buf, BUF_SIZE);
  if (path == nullptr) {
    return "";
  }
  return path;
}


void makeDir(const string& path) {
  /*int status = */mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
}


string getFilename(const string& path) {
  auto pos = path.find_last_of('/');
  if (pos == string::npos) return "";

  // skip / by + 1
  return path.substr(pos + 1);
}


string getExtension(const string& filename) {
  auto pos = filename.find_last_of('.');
  if (pos == string::npos) return "";

  // skip . by + 1
  return filename.substr(pos + 1);
}


string randomFilename(const string& filename) {
  string ext = getExtension(filename);

  string name = uuid4();

  return ext.length() > 0 ? name + '.' + ext : name;
}


void copyFile(const string& dest, const string& src) {
  std::ifstream fs (src, std::fstream::binary);
  std::ofstream fd (dest, std::fstream::trunc | std::fstream::binary);
  fd << fs.rdbuf ();
}


// TODO: not working
void moveFile(const string& dest, const string& src) {
  std::rename(src.data(), dest.data());
}


void tolower(string& value) {
  std::transform(value.begin(), value.end(), value.begin(), ::tolower);
}


string encodeUrl(const string& value) {
  ostringstream escaped;
  escaped.fill('0');
  escaped << std::hex << std::uppercase;

  std::for_each(value.begin(), value.end(), [&escaped](string::value_type c) {
    // keep alphanumeric and other accepted characters intact
    if (std::isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
      escaped << c;
    } else {
      // any other characters are percent-encoded
      escaped << '%' << std::setw(2) << int((unsigned char) c);
    }
  });

  return escaped.str();
}


string decodeUrl(const string& value) {
  ostringstream unescaped;

  char buf[3];
  for (size_t i = 0, n = value.length(); i < n; ++i) {
    if (value[i] == '%' && (i + 2) < n && std::isxdigit(value[i+1]) && std::isxdigit(value[i+2])) {
      buf[0] = value[++i];
      buf[1] = value[++i];
      buf[2] = '\0';
      unescaped << ((char)std::strtol(buf, nullptr, 16));
    } else {
      unescaped << value[i];
    }
  }

  return unescaped.str();
}


string escape(const string& value) {
  ostringstream escaped;
  string::value_type c;
  for (auto i = value.begin(), n = value.end(); i != n; ++i) {
    c = *i;
    switch (c) {
      case '<':
        escaped << "&lt";
        break;
      case '>':
        escaped << "&gt";
        break;
      case '&':
        escaped << "&amp";
        break;
      case '"':
        escaped << "&quot";
        break;
      default:
        escaped << c;
    }
  }
  return escaped.str();
}


string escapeJsonString(const string& value) {
  ostringstream buf;

  for (auto c: value) {
    switch (c) {
      case '\\': buf << "\\\\"; break;
      case '"': buf << "\\\""; break;
      case '/': buf << "\\/"; break;
      case '\b': buf << "\\b"; break;
      case '\f': buf << "\\f"; break;
      case '\n': buf << "\\n"; break;
      case '\r': buf << "\\r"; break;
      case '\t': buf << "\\t"; break;
      default: buf << c; break;
    }
  };

  return buf.str();
}


static const string base64_chars = 
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789+/";


static inline bool is_base64(unsigned char c) {
  return (isalnum(c) || (c == '+') || (c == '/'));
}


string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len) {
  string ret;
  int i = 0;
  int j = 0;
  unsigned char char_array_3[3];
  unsigned char char_array_4[4];

  while (in_len--) {
    char_array_3[i++] = *(bytes_to_encode++);
    if (i == 3) {
      char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
      char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
      char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
      char_array_4[3] = char_array_3[2] & 0x3f;

      for(i = 0; (i <4) ; i++)
        ret += base64_chars[char_array_4[i]];
      i = 0;
    }
  }

  if (i) {
    for(j = i; j < 3; j++)
      char_array_3[j] = '\0';

    char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
    char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
    char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
    char_array_4[3] = char_array_3[2] & 0x3f;

    for (j = 0; (j < i + 1); j++)
      ret += base64_chars[char_array_4[j]];

    while((i++ < 3))
      ret += '=';
  }

  return ret;
}


string base64_decode(string const& encoded_string) {
  int in_len = encoded_string.size();
  int i = 0;
  int j = 0;
  int in_ = 0;
  unsigned char char_array_4[4], char_array_3[3];
  string ret;

  while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
    char_array_4[i++] = encoded_string[in_]; in_++;
    if (i ==4) {
      for (i = 0; i <4; i++)
        char_array_4[i] = base64_chars.find(char_array_4[i]);

      char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
      char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
      char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

      for (i = 0; (i < 3); i++)
        ret += char_array_3[i];
      i = 0;
    }
  }

  if (i) {
    for (j = i; j <4; j++)
      char_array_4[j] = 0;

    for (j = 0; j <4; j++)
      char_array_4[j] = base64_chars.find(char_array_4[j]);

    char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
    char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
    char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

    for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
  }

  return ret;
}


string createSignature(const string& secret,
    const string& toSign) {
  SHA256_CTX ctx;
  unsigned char hash[SHA256_DIGEST_LENGTH];

  SHA256_Init(&ctx);
  SHA256_Update(&ctx, secret.data(), secret.length());
  SHA256_Update(&ctx, toSign.data(), toSign.length());
  SHA256_Final(hash, &ctx);

  ostringstream buf;
  buf << std::hex << std::setfill('0');
  for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
    buf << std::setw(2) << int(hash[i]);
  }

  return buf.str();
}


string createSignedValue(const string& secret,
    const string& name, const string& value, time_t clock) {
  string clockStr = std::to_string(clock);
  string value64 = base64Encode(value);

  ostringstream buf;

  // format version, key version, for key rotations
  buf  << "1|1:0|" 
    << clockStr.length() << ':' << clockStr << '|'
    << name.length() << ':' << name << '|'
    << value64.length() << ':' << value64;

  string toSign = buf.str();

  buf << '|' << createSignature(secret, toSign);

  return buf.str();
}


string consumeField(const string& field) {
  auto colon = std::find(field.begin(), field.end(), ':');
  if (colon == field.end()) {
    return "";
  }

  // check value length
  string lengthValue(field.begin(), colon);
  int length = 0;

  try {
    length = std::stoi(lengthValue);
  } catch (...) {
    return "";
  }

  if (field.end() - colon - 1 != length) {
    return "";
  }

  string value(colon + 1, field.end());

  return value;
}


string decodeSignedValue(const string& secret, const string& name,
    const string& value, int maxAgeDays, time_t /*clock*/) {
  // parse version
  auto versionEnd = std::find(value.begin(), value.end(), '|');
  if (versionEnd == value.end()) {
    return "";
  }

  // parse minor version
  auto keyEnd = std::find(versionEnd + 1, value.end(), '|');
  if (keyEnd == value.end()) {
    return "";
  }

  // parse time
  auto timeEnd = std::find(keyEnd + 1, value.end(), '|');
  if (timeEnd == value.end()) {
    return "";
  }

  // parse name
  auto nameEnd = std::find(timeEnd + 1, value.end(), '|');
  if (nameEnd == value.end()) {
    return "";
  }

  // parse value
  auto valueEnd = std::find(nameEnd + 1, value.end(), '|');
  if (valueEnd == value.end()) {
    return "";
  }

  // check signature
  string toSign(value.begin(), valueEnd);
  string signature = createSignature(secret, toSign);
  string signatureValue(valueEnd + 1, value.end());

  if (signature != signatureValue) {
    return "";
  }

  string timeField(keyEnd + 1, timeEnd);
  string timeValue = consumeField(timeField);
  if (timeValue.length() == 0) {
    return "";
  }

  time_t timeRec = 0;
  try {
    timeRec = std::stol(timeValue);
  } catch (...) {
    return "";
  }

  // check expires
  if (timeRec < std::time(nullptr) - maxAgeDays * 86400) {
    return "";
  }

  string nameField(timeEnd + 1, nameEnd);
  string nameValue = consumeField(nameField);
  if (nameValue != name) {
    return "";
  }

  // encode value64
  string valueField(nameEnd + 1, valueEnd);
  string value64 = consumeField(valueField);
  string res = base64Decode(value64);

  return res;
}


string base64Encode(const string& value) {
  return base64_encode(reinterpret_cast<const unsigned char*>(value.data()), value.length());
}


string base64Decode(const string& value) {
  return base64_decode(value);
}


string encodeCookie(const string& secret,
    const string& name, const string& value, time_t clock) {
  return createSignedValue(secret, name, value, clock);
}


string decodeCookie(const string& secret,
    const string& name, const string& value, int maxAgeDays, time_t clock) {
  return decodeSignedValue(secret, name, value, maxAgeDays, clock);
}


string urandom(size_t n) {
  char buf[n];

  std::ifstream gen("/dev/urandom", std::ios::in|std::ios::binary);
  gen.read(buf, n);
  gen.close();

  ostringstream res;
  res << std::hex << std::setfill('0');
  for (size_t i = 0; i < n; ++i) {
    res << std::setw(2) << int((unsigned char)buf[i]);
  }

  return res.str();
}


string uuid4() {
  size_t n = 36; // uuid4 length
  char buf[n];

  std::ifstream gen("/proc/sys/kernel/random/uuid");//, std::ios::in|std::ios::binary);
  gen.read(buf, n);
  gen.close();

  string result(&buf[0], &buf[n]);

  return result;
}


string createXsrf() {
  ostringstream buf;
  string token = urandom(16);

  // format version, clock, token
  buf << "1|"
    << std::to_string(std::time(nullptr)) << '|'
    << token;

  return buf.str();
}


string decodeXsrf(const string& value) {
  auto versionEnd = std::find(value.begin(), value.end(), '|');
  if (versionEnd == value.end()) {
    return "";
  }

  auto clockEnd = std::find(versionEnd + 1, value.end(), '|');
  if (clockEnd == value.end()) {
    return "";
  }

  string clockValue(versionEnd + 1, clockEnd);
  if (clockValue.length() == 0) {
    return "";
  }

  /*
  time_t clock = 0;
  try {
    clock = std::stol(clockValue);
  } catch (...) {
    return "";
  }

  // check expires
  if (clock < std::time(nullptr) - maxAgeDays * 86400) {
    return "";
  }
  */

  string token(clockEnd + 1, value.end());
  return token;
}


string createPassword() {
  string rand = uuid4() + uuid4();
  return base64Encode(rand);
}


string encodeSaltValue(const string& salt, const string& value) {
  SHA_CTX context;
  unsigned char hash[SHA_DIGEST_LENGTH];

  SHA1_Init(&context);
  SHA1_Update(&context, salt.data(), salt.length());
  SHA1_Update(&context, value.data(), value.length());
  SHA1_Final(hash, &context);

  ostringstream buf;

  buf << std::hex << std::setfill('0');
  for (int i = 0; i < SHA_DIGEST_LENGTH; ++i) {
    buf << std::setw(2) << int(hash[i]);
  }

  return buf.str();
}


string createSalt() {
  return urandom(7);
}


string encodePassword(const string& raw) {
  ostringstream buf;

  string salt = createSalt();

  buf << salt << '$' << encodeSaltValue(salt, raw);

  return buf.str();
}


bool checkPassword(const string& raw, const string& encoded) {
  // extract salt from encoded
  auto dol = std::find(encoded.begin(), encoded.end(), '$');
  if (dol == encoded.end()) {
    return false;
  }

  string salt(encoded.begin(), dol);
  string expected(dol+1, encoded.end());

  // encode raw password with salt
  string actual = encodeSaltValue(salt, raw);

  return actual == expected;
}


string unquote(string& source) {
  size_t lpos = 0;
  size_t rpos = source.length();

  if (source[lpos] == '"') ++lpos;
  if (source[rpos - 1] == '"') --rpos;

  return string(source, lpos, rpos - lpos);
}


// split src by delim
vector<string> parseGeneric(string const& src, const char delim) {
  vector<string> items = {};

  int length = src.length();
  int left = 0;
  int right = 0;

  for (; right < length; ++right) {
    if (src[right] == delim and right - left >= 0) {
      items.push_back(src.substr(left, right - left));
      left = right + 1; // move begin after delim
    }
  }

  // add last field
  if (length > 0 and length - left >= 0) {
    string item = src.substr(left, length - left);
    items.push_back(item);
  }

  return items;
}


vector<string> parseRows(string const& src) {
  return parseGeneric(src, DELIM_ROW);
}


vector<string> parseFields(string const& src) {
  return parseGeneric(src, DELIM_FIELD);
}


void insertItem(string& items, string& beforeId, string& id) {
  if (beforeId.empty()) {
    appendItem(items, id);
    return;
  }

  // split items by :
  auto ids = parseGeneric(items, ':');

  vector<string> filtered;
  for (size_t i = 0, size = ids.size(); i < size; ++i) {
    if (ids[i] == beforeId) {
      filtered.push_back(id);
    }
    filtered.push_back(ids[i]);
  }

  // join buf
  ostringstream result;
  // put first
  if (filtered.size() > 0) result << filtered[0];
  for (size_t i = 1, size = filtered.size(); i < size; ++i) {
    result << ':' << filtered[i];
  }

  items = result.str();
}


void appendItem(string& items, string& id) {
  if (items.empty()) items = id;
  else items = items + ':' + id;
}


void removeItem(string& items, string& id) {
  auto ids = parseGeneric(items, ':');

  vector<string> filtered;
  for (size_t i = 0, size = ids.size(); i < size; ++i) {
    if (ids[i] != id) 
      filtered.push_back(ids[i]);
  }

  ostringstream result;
  if (filtered.size() > 0) result << filtered[0];
  for (size_t i = 1, size = filtered.size(); i < size; ++i) {
    result << ':' << filtered[i];
  }

  items = result.str();
}


void moveItem(string& items, string& beforeId, string& id) {
  auto ids = parseGeneric(items, ':');

  vector<string> filtered;
  for (size_t i = 0, size = ids.size(); i < size; ++i) {
    if (ids[i] == beforeId) {
      filtered.push_back(id);
    }
    if (ids[i] != id)
      filtered.push_back(ids[i]);
  }

  ostringstream result;
  if (filtered.size() > 0) result << filtered[0];
  for (size_t i = 1, size = filtered.size(); i < size; ++i) {
    result << ':' << filtered[i];
  }

  items = result.str();
}


void split(std::string& text, char delimiter,
    std::vector<std::string>& chunks) {
  int left = 0;
  bool isDelimiterFound = false;

  for (size_t i = 0; i < text.size(); ++i) {
    if (text[i] == delimiter) {
      isDelimiterFound = true;
      chunks.push_back(text.substr(left, i - left));
      left = i + 1;
    }
  }

  // add remainder
  if (isDelimiterFound) {
    chunks.push_back(text.substr(left, text.size() - left));
  }
}


std::string join(std::vector<std::string>& words, char delimiter) {
  std::ostringstream buf;

  if (words.empty()) return "";
  if (words.size() == 1) return words[0];

  buf << words[0];
  for (size_t i = 1; i < words.size(); ++i) {
    buf << delimiter << words[i];
  }

  return buf.str();
}


} /* namespace shot */
