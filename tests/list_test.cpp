#include "MCache.h"
#include <cassert>

int main() {
  MCache cache;

  assert(cache.push_list("a", MCache::ValueType::INT, "1 2 3 4 5").success == true);
  assert(cache.push_list("a", MCache::ValueType::STRING, "2").success == false);
  assert(cache.push_list("a", MCache::ValueType::INT, "6").success == true);
  assert(cache.push_list("a",MCache::ValueType::STRING, "2").success == false);

  MCache::Response val = cache.get_val("a");
  assert(val.success && val.type == "list");
  // assert(val.success && val.type == "string" && std::visit([](auto& v) { 
  //   if constexpr (std::is_same_v<std::decay_t<decltype(v)>, std::string>) {
  //       return v == "2";
  //   } else {
  //       return false;
  //   }
  // }, *val.data));
  assert(cache.del_key("a") == true);
  assert(cache.get_val("a").success == false);

  return 0;
}
