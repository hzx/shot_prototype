#include "values.h"

namespace shot {


void Int::set(int value_) {
  value = value_;
  has = true;
}


void Float::set(float value_) {
  value = value_;
  has = true;
}


void String::set(string& value_) {
  value = value_;
  has = true;
}


void String::set(string&& value_) {
  value = value_;
  has = true;
}


void String::set(string const& value_) {
  value = value_;
  has = true;
}

  
} /* namespace shot */
