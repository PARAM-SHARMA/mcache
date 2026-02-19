#include "MCache.h"
#include <iostream>

MCache::MCache() {
  this->hits = 0;
  this->misses = 0;
  this->keys = 0;
}

std::optional<int> MCache::get_val(const std::string& key) {
  auto it = map.find(key);
  if (it == map.end()) {
    this->misses++;
    return std::nullopt;
  } else {
    this->hits++;
    return it->second;
  }
}

bool MCache::add(const std::string& key, const int value) {
  std::lock_guard<std::mutex> lock(mtx);
  auto it = map.find(key);
  if (it != map.end()) {
    return false;
  }
  this->keys++;
  map.insert({ key, value });
  return true;
}

bool MCache::set_val(const std::string& key, const int value) {
  std::lock_guard<std::mutex> lock(mtx);
  auto it = map.find(key);
  if (it == map.end()) {
    return false;
  }
  it->second = value;
  return true;
}

bool MCache::del_val(const std::string& key) {
  std::lock_guard<std::mutex> lock(mtx);
  if (map.find(key) == map.end()) {
    return false;
  }

  map.erase(key);
  return true;
}

std::unordered_map<std::string, int> MCache::stats() {
  std::unordered_map<std::string, int> stat;

  stat.insert({ "hits", this->hits });
  stat.insert({ "misses", this->misses });
  stat.insert({ "keys", this->keys });

  return stat;
}
