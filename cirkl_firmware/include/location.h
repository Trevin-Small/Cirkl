#pragma once

#include <string>

typedef struct location {
  std::string lat;
  std::string lon;
} loc_t;

extern loc_t location;