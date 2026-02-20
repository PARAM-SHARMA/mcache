#pragma once

#include "MCache.h"
#include <string>

class CLI {
  MCache& cache_;

public:

  CLI(MCache& cache) : cache_(cache) {}
  void run();
};
