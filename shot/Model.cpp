#include <sstream>
#include <iostream>
#include "Model.h"
#include "utils.h"
#include "http.h"


namespace shot {


int Model::fromCompactFormat(string const& raw) {
  auto fields = parseFields(raw);
  size_t size = fields.size();

  if (size < 2) return -1;

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


int Model::fromCustomCompactFormat(string const& raw, char delimiter) {
  std::vector<std::string> pices;
  shot::split(raw, delimiter, pices);
  size_t size = pices.size();

  if (size < 2) return -1;
  if (size % 2 != 0) --size;

  int error = 0;
  int code = 0;

  for (size_t i = 0; i < size; i += 2) {
    try {
      code = std::stoi(pices[i]);
    } catch (...) {
      return -1;
    }

    error = parseField(code, pices[i+1]);
    if (error) break;
  }

  return error;
}


} /* namespace shot */
