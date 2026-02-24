#include "MCache.h"
#include "serialization.h"

std::optional<std::pair<std::string, std::string>> MCache::get_val(const std::string& key) {
  std::lock_guard<std::mutex> lock(mtx_);
  auto it = store_.find(key);
  if (it == store_.end()) {
    ++misses_;
    return std::nullopt;
  } else {
    ++hits_;
    std::pair<std::string, std::string> c_val;
    if (it->second.type == MCache::ValueType::INT) {
      int value = serialization::from_bytes<int>(it->second.data); 
      c_val.first = "int";
      c_val.second = std::to_string(value);
    }
    else if (it->second.type == MCache::ValueType::FLOAT) {
      float value = serialization::from_bytes<float>(it->second.data); 
      c_val.first = "float";
      c_val.second = std::to_string(value);
    }
    else if (it->second.type == MCache::ValueType::STRING) {
      std::string value = serialization::from_bytes_string(it->second.data); 
      c_val.first = "string";
      c_val.second = value;
    }
    return c_val;
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

bool MCache::set_val(const std::string& key, const std::string& type, const std::string& value) {
  std::lock_guard<std::mutex> lock(mtx_);
  auto it = store_.find(key);
  if (it == store_.end()) {
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
  it->second = c_val;
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
