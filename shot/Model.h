#ifndef SHOT_MODEL_H
#define SHOT_MODEL_H


#include <string>
#include <vector>


using std::string;
using std::vector;


namespace shot {


class Model {
public:
  virtual string serializeValue() = 0;
  virtual string serialize() = 0;
  virtual string unserializeValue() = 0;
  virtual string unserialize() = 0;
};

  
} /* namespace shot */


#endif /* end of include guard: SHOT_MODEL_H */
