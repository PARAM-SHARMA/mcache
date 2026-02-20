#pragma once

#include <unordered_map>
#include <string>
#include <optional>
#include <mutex>

class MCache {
private:
  std::unordered_map<std::string, int> store_;
  mutable std::mutex mtx_;
  size_t hits_ = 0;
  size_t misses_ = 0;
  
public:

  MCache() = default;
  MCache(const MCache&) = delete;
  MCache operator=(const MCache&) = delete;

  std::optional<int> get_val(const std::string& key);
  bool add_val(const std::string& key, const int value);
  bool set_val(const std::string& key, const int value);
  bool del_val(const std::string& key) noexcept;

  struct Stats {
    size_t hits;
    size_t misses;
    size_t keys;
  };

  Stats get_stats() const;
};
