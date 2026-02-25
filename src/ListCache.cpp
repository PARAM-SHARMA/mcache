#include "MCache.h"
#include "serialization.h"

bool MCache::push_list(const std::string& key, const std::string& type, const std::string& values) {
  std::lock_guard<std::mutex> lock(mtx_);
  MCache::ByteList bl;


  serialization::construct_list(bl,values);

  auto it = store_.find(key);
  if (it != store_.end()) {
    if (auto* existing_list = std::get_if<ByteList>(&it->second.data)) {
      existing_list->insert(existing_list->end(), bl.begin(), bl.end());
    } else {
      return false;
    }

  } else {
    CacheValue cv;

    if (type == "int") {
      cv.type = MCache::ValueType::INT;
    } else if (type == "float") {
      cv.type = MCache::ValueType::FLOAT;
    } else if (type == "string") {
      cv.type = MCache::ValueType::STRING;
    }

    cv.s_type = MCache::StructType::LIST;
    cv.data = std::move(bl);
    store_[key] = std::move(cv);
  }

  return true;


}
