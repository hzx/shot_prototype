#ifndef SHOT_DB_TYPES_H
#define SHOT_DB_TYPES_H


#include <string>
#include <vector>
#include <ostream>
#include <ctime>


using std::string;


namespace shot {


int const FIELD_TYPE_INT = 1;
int const FIELD_TYPE_FLOAT = 2;
int const FIELD_TYPE_STRING = 3;
int const FIELD_TYPE_DATE = 4;
int const FIELD_TYPE_TDATE = 5;
int const FIELD_TYPE_FILE = 6;
int const FIELD_TYPE_IMAGE = 7;
int const FIELD_TYPE_TAGS = 8;
int const FIELD_TYPE_SLUG = 9;


int const FIELD_DEFAULT_INT = 0;
float const field_default_float = 0.0;
string const field_default_string = "";
string const field_default_date = "1970-01-01 00:00:00";
time_t const field_default_tdate = 0;
string const field_default_image = "";


class Param {
public:
  virtual void out(std::ostream& o) = 0;
};


class Int {
public:
  Int();
  void out (std::ostream& o);
  int code;
  int value;
};


class Float {
public:
  Float();
  void out (std::ostream& o);
  int code;
  float value;
};


class String {
public:
  String();
  void out (std::ostream& o);
  int code;
  string value;
};


class Date {
public:
  Date();
  void out (std::ostream& o);
  int code;
  string value;
};


class Tdate {
public:
  Tdate();
  void out (std::ostream& o);
  int code;
  time_t value;
};


class Image {
public:
  Image();
  void out (std::ostream& o);
  int code;
  string value;
};


} /* namespace shot */


#endif /* end of include guard: SHOT_DB_TYPES_H */
