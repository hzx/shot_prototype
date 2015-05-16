#ifndef SHOT_VALUES_H
#define SHOT_VALUES_H

#include <string>

using std::string;

namespace shot {
  

class Int {
public:
  int value = 0;
  bool has = false;

  void set(int value_);
};


class Float {
public:
  float value = 0.0;
  bool has = false;

  void set(float value_);
};


class String {
public:
  string value;
  bool has = false;

  void set(string& value_);
  void set(string&& value_);
  void set(string const& value_);
};


} /* namespace shot */

#endif /* end of include guard: SHOT_VALUES_H */
