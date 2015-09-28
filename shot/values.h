#ifndef SHOT_VALUES_H
#define SHOT_VALUES_H

#include <string>

using std::string;

namespace shot {
  

class Int {
public:
  int value = 0;
  bool has = false;

  void set(int value);
};


class Uint {
public:
  unsigned int value = 0;
  bool has = false;

  void set(uint value);
};


class Float {
public:
  float value = 0.0;
  bool has = false;

  void set(float value);
};


class String {
public:
  string value;
  bool has = false;

  void set(string& value);
  void set(string&& value);
  void set(string const& value);
};


} /* namespace shot */

#endif /* end of include guard: SHOT_VALUES_H */
