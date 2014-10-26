#ifndef SHOT_MODEL_H
#define SHOT_MODEL_H


#include <string>


using std::string;


namespace shot {


class Model {
public:
  virtual string toJsonString() = 0;
};

  
} /* namespace shot */


#endif /* end of include guard: SHOT_MODEL_H */
