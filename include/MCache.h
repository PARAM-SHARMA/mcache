#pragma once

#include <unordered_map>
#include <string>
#include <optional>
#include <mutex>
#include <vector>

class MCache {
private:
  enum class ValueType { INT, FLOAT, STRING };
  struct CacheValue {
    ValueType type;
    std::vector<uint8_t> data;
  };
  std::unordered_map<std::string, CacheValue> store_;
  mutable std::mutex mtx_;
  size_t hits_ = 0;
  size_t misses_ = 0;
  
public:

  MCache() = default;
  MCache(const MCache&) = delete;
  MCache operator=(const MCache&) = delete;

  std::optional<CacheValue> get_val(const std::string& key);
  bool add_val(const std::string& key, const std::string& type, const std::string& value);
  bool set_val(const std::string& key, const std::vector<uint8_t>& value);
  bool del_val(const std::string& key) noexcept;

  struct Stats {
    size_t hits;
    size_t misses;
    size_t keys;
  };

  Stats get_stats() const;
};
