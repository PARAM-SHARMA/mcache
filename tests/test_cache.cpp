#include "MCache.h"
#include <cassert>

int main() {
  MCache cache;

  assert(cache.add_val("a", "int", "1") == true);
  // assert(cache.add_val("a", "string", 2) == false);
  // assert(cache.set_val("a", 3) == true);
  // auto val = cache.get_val("a");
  // assert(val && *val == 3);
  // assert(cache.del_val("a") == true);
  // assert(cache.get_val("a") == std::nullopt);

  return 0;
}
