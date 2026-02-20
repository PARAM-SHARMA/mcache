#include "MCache.h"

std::optional<int> MCache::get_val(const std::string& key) {
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

bool MCache::add_val(const std::string& key, const int value) {
  std::lock_guard<std::mutex> lock(mtx_);
  auto it = store_.find(key);
  if (it != store_.end()) {
    return false;
  }
  store_.emplace(key, value);
  return true;
}

bool MCache::set_val(const std::string& key, const int value) {
  std::lock_guard<std::mutex> lock(mtx_);
  auto it = store_.find(key);
  if (it == store_.end()) {
    return false;
  }
  it->second = value;
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
