#include "shot/url.h"
#include "shot/Config.h"
#include "shot/utils.h"


namespace shot {


string staticUrl(string filename) {
  auto path = shot::Config::instance().directory + filename;
  return !pathExists(path.c_str()) ? "" :
    filename + "?v=" + pathMd5(path.c_str());
}

  
} /* namespace shot */
