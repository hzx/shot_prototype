#include <sstream>
#include <iostream>
#include "Model.h"
#include "utils.h"
#include "http.h"



namespace shot {


int Model::initRaw(string const& raw) {
  auto fields = parseFields(raw);
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
