#ifndef SHOT_OPTIONS_H
#define SHOT_OPTIONS_H

#include <string>


using std::string;


namespace shot {


const int DEFAULT_PORT = 8000;
const int DEFAULT_MAX_EVENTS = 1024;
const int DEFAULT_CHUNK_SIZE = 16384; // 1024 * 16 = 16KB, MAX = 65535
const string DEFAULT_OPTIONS_FILENAME = "/etc/shot/shot.conf";


class Options {
public:
  Options();
  ~Options();

  static Options& instance();

  // listened port
  int port;
  // max epoll events
  int maxEvents;
  // socket read chunk size
  int chunkSize;
  // config filename
  string filename;
  // current working directory
  string directory;
  // static directory
  string staticDir;
  string imgDir;
  string dataDir;
  // temprorary folder, used for request.files
  string tmp;
  string host;
  string cookieSecret;
  string dbhost;
  string dbport;
  string dbuser;
  string dbpassword;
  string dbname;

  int parseCmd(int argc, const char* argv[]);
  int parseFile(const char* filename);
};


} /* namespace shot */

#endif /* end of include guard: SHOT_OPTIONS_H */
