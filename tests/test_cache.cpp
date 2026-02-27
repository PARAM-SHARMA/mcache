#include "MCache.h"
#include <cassert>

int main() {
  MCache cache;

  assert(cache.add_val("a", MCache::ValueType::INT, "1").success == true);
  assert(cache.add_val("a", MCache::ValueType::STRING, "2").success == false);
  assert(cache.add_val("a", MCache::ValueType::INT, "test").success == false);
  assert(cache.set_val("a",MCache::ValueType::STRING, "2").success == true);

  MCache::Response val = cache.get_val("a");
  assert(val.success && val.type == "string" && std::visit([](auto& v) { 
    if constexpr (std::is_same_v<std::decay_t<decltype(v)>, std::string>) {
        return v == "2";
    } else {
        return false;
    }
  }, *val.data));
  assert(cache.del_key("a") == true);
  assert(cache.get_val("a").success == false);

  return 0;
}
