#include <sstream>
#include <iostream>
#include "Model.h"
#include "utils.h"
#include "http.h"



namespace shot {


string Model::toString(vector<int> codes) {
  std::ostringstream b;
  bool first = true;

  for (int code: codes) {
    // add delimiter before next pair
    if (first) first = false; else b << DELIM_FIELD;
    b << code << DELIM_FIELD;
    storeField(b, code);
  }

  return b.str();
}


int Model::fromString(string const& src) {
  auto fields = parseFields(src);
  size_t size = fields.size();

  // correct size to even
  if (size % 2 != 0) --size;

  int error = 0;
  int code = 0;

  for (size_t i = 0; i < size; i += 2) {
    // name to name code
    try {
      code = std::stoi(fields[i]);
    } catch (...) {
      return -1;
    }

    error = parseField(code, fields[i+1]);
    if (error) break;
  }

  return error;
}

  
} /* namespace shot */
