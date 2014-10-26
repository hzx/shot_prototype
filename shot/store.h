#ifndef SHOT_STORE_H
#define SHOT_STORE_H


#include <ctime>


namespace shot {


enum class FieldType: int {
  INT = 1,
  FLOAT = 2,
  STRING = 3,
  DATE = 4,
  TDATE = 5,
  IMAGE = 6
};


class FieldDefault {
public:
  int INT = 0;
  float FLOAT = 0;
  string STRING = 0;
  string DATE = "1970-01-01 00:00:00";
  time_t TDATE = 0;
  string IMAGE = "";
};


class Field {
public:
  FieldType type;
  FieldDefault def;
};

  
} /* namespace shot */


#endif /* end of include guard: SHOT_STORE_H */
