#ifndef SHOT_MODEL_H
#define SHOT_MODEL_H


#include <ostream>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include "DbClient.h"
#include "http.h"

using std::string;
using std::vector;
using std::ostream;
using std::ostringstream;


namespace shot {


class Model {
public:
  virtual int init(bson::bo& obj) = 0;
  virtual int parseField(int code, string const& value) = 0;
  virtual void save(bson::bob& builder) = 0;
  virtual void save(ostream& stream) = 0;
  int initRaw(string const& raw);
};


template<class T>
int cursorToStream(mongo::DBClientCursor& cursor, ostream& stream) {
  int counter = 0;

  while (cursor.more()) {
    ++counter;
    T model;
    bson::bo obj = cursor.next();
    model.init(obj);
    model.save(stream);
    stream << shot::DELIM_ROW;
  }

  return counter;
}


template<class T>
int tableToStream(int table, mongo::DBClientCursor& cursor, ostream& stream) {
  ostringstream buf;
  int count = cursorToStream<T>(cursor, buf);
  if (count > 0) {
    stream << table << shot::DELIM_FIELD << count << shot::DELIM_ROW << buf;
  }
  return count;
}

  
} /* namespace shot */


#endif /* end of include guard: SHOT_MODEL_H */
