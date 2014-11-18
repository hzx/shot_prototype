#ifndef SHOT_DB_H
#define SHOT_DB_H


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


#endif /* end of include guard: SHOT_DB_H */
