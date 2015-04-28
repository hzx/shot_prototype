#include "url.h"
#include "Options.h"
#include "utils.h"


namespace shot {


string staticUrl(string filename) {
  auto path = shot::Options::instance().directory + filename;
  return !pathExists(path.c_str()) ? "" :
    filename + "?v=" + pathMd5(path.c_str());
}

  
} /* namespace shot */
