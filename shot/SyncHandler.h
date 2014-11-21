#ifndef SHOT_SYNC_HANDLER_H
#define SHOT_SYNC_HANDLER_H


#include "Handler.h"


namespace shot {


int const SYNC_OP = 1;
int const SYNC_NAME = 2;
int const SYNC_PARAMS = 3;
int const SYNC_BEFOREID = 4;
int const SYNC_OBJ = 5;
int const SYNC_ID = 6;

int const SYNC_OP_NONE = 0;
int const SYNC_OP_UPDATE = 1;
int const SYNC_OP_INSERT = 2;
int const SYNC_OP_APPEND = 3;
int const SYNC_OP_REMOVE = 4;
int const SYNC_OP_MOVE = 5;


class SyncHandler: public Handler {
public:
  void xpost();
  void opUpdate();
  void opInsert();
  void opAppend();
  void opRemove();
  void opMove();
};

  
} /* namespace shot */


#endif /* end of include guard: SHOT_SYNC_HANDLER_H */
