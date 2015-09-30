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
  void set(std::string value);
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

class Bool {
public:
  bool value = false;
  bool has = false;

  void set(bool value);
  void on();
  void off();
};


} /* namespace shot */

#endif /* end of include guard: SHOT_VALUES_H */
