#include "values.h"
#include <cstdlib>


namespace shot {


void Int::set(int value_) {
  value = value_;
  has = true;
}


void Uint::set(uint value_) {
  value = value_;
  has = true;
}


void Uint::set(std::string value) {
  value = std::strtoul(value.c_str(), nullptr, 10);
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
