#include "MCache.h"
#include "serialization.h"

std::optional<MCache::CacheValue> MCache::get_val(const std::string& key) {
  std::lock_guard<std::mutex> lock(mtx_);
  auto it = store_.find(key);
  if (it == store_.end()) {
    ++misses_;
    return std::nullopt;
  } else {
    ++hits_;
    return it->second;
  }
}

bool MCache::add_val(const std::string& key, const std::string& type, const std::string& value) {
  std::lock_guard<std::mutex> lock(mtx_);
  auto it = store_.find(key);
  if (it != store_.end()) {
    return false;
  }
  MCache::CacheValue c_val;
  if (type == "int") {
      c_val.type = ValueType::INT;
      c_val.data = serialization::to_bytes(std::stoi(value));
  }
  else if (type == "float") {
      c_val.type = ValueType::FLOAT;
      c_val.data = serialization::to_bytes(std::stof(value));
  }
  else if (type == "string") {
      c_val.type = ValueType::STRING;
      c_val.data = serialization::to_bytes(value);
  }
  else {
      return false;
  }
  store_.emplace(key, c_val);
  return true;
}

bool MCache::set_val(const std::string& key, const std::vector<uint8_t>& value) {
  std::lock_guard<std::mutex> lock(mtx_);
  auto it = store_.find(key);
  if (it == store_.end()) {
    return false;
  }
  // it->second = value;
  return true;
}

bool MCache::del_val(const std::string& key) noexcept {
  std::lock_guard<std::mutex> lock(mtx_);
  return store_.erase(key) > 0;
}

MCache::Stats MCache::get_stats() const {
  std::lock_guard<std::mutex> lock(mtx_);

  return Stats {
    hits_,
    misses_,
    store_.size()
  };
}
