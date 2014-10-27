#include "url.h"
#include "Config.h"
#include "utils.h"


namespace shot {


string staticUrl(string filename) {
  auto path = shot::Config::instance().directory + filename;
  return !pathExists(path.c_str()) ? "" :
    filename + "?v=" + pathMd5(path.c_str());
}

  
} /* namespace shot */
