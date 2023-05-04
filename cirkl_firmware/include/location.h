#pragma once

#include <string>

typedef struct location {
  bool is_known;
  std::string lat;
  std::string lon;
} loc_t;

extern loc_t location;