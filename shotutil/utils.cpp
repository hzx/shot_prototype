#include <unistd.h>
#include <dirent.h>
#include <fstream>
#include "utils.h"

using std::ifstream;


const size_t BUF_SIZE = 4096;


string getCurrentDir() {
  char buf[BUF_SIZE];
  char* path = getcwd(buf, BUF_SIZE);
  if (path == nullptr) {
    return "";
  }
  return path;
}


bool existsFile(string& filename) {
  ifstream inp(filename);
  return inp.good();
}
