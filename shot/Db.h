#pragma once
#include "db_types.h"


namespace shot {


class Db {
public:
  void insert();
  void update();
  void remove();
  void find();
  void move();
};


} /* namespace shot */
