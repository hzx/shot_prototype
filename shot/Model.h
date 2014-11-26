#ifndef SHOT_MODEL_H
#define SHOT_MODEL_H


#include <ostream>
#include <string>
#include <vector>


using std::string;
using std::vector;


namespace shot {


class Model {
public:
  virtual int parseField(int code, string const& value) = 0;
  virtual void storeField(std::ostream& b, int code) = 0;
  string toString(vector<int> codes);
  int fromString(string const& src);
};

  
} /* namespace shot */


#endif /* end of include guard: SHOT_MODEL_H */
