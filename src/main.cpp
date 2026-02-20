#include "MCache.h"
#include "cli.h"


int main() {
  MCache cache;
  CLI cli(cache);
  cli.run();

  return 0;
}
