#pragma once

#include <unordered_map>
#include <unordered_set>
#include <string>
#include <optional>
#include <mutex>
#include <vector>
#include <variant>

class MCache {
public:
  enum class ValueType { INT, FLOAT, STRING };
private:
  enum class StructType { RAW, LIST, SET, MAP };
  using Bytes = std::vector<uint8_t>;

  struct BytesHash {
      size_t operator()(const Bytes& v) const noexcept {
          size_t h = 0;
          for (auto b : v) h = h * 131 + b;
          return h;
      }
  };

  using ByteList = std::vector<Bytes>;
  using ByteSet  = std::unordered_set<Bytes, BytesHash>;
  using ByteMap  = std::unordered_map<std::string, Bytes>;
  struct CacheValue {
    ValueType type;
    StructType s_type;
    std::variant<
      Bytes,
      ByteList,
      ByteMap,
      ByteSet
    > data;
  };
  std::unordered_map<std::string, CacheValue> store_;
  mutable std::mutex mtx_;
  size_t hits_   = 0;
  size_t misses_ = 0;
  
public:

  MCache() = default;
  MCache(const MCache&) = delete;
  MCache operator=(const MCache&) = delete;

  struct Response {
    bool success;
    std::string s_type;
    std::string type;
    std::optional<std::variant<
      int, float, std::string, std::vector<int>, std::vector<float>, std::vector<std::string>
    >> data;
    std::string error;
  };

  // Delete types of keys
  bool del_key(const std::string& key) noexcept;

  // Handle raw key value pair
  Response get_val(const std::string& key);
  Response add_val(const std::string& key, const ValueType type, const std::string& value);
  Response set_val(const std::string& key, const ValueType type, const std::string& value);
  std::optional<MCache::CacheValue> parse_value(const ValueType type, const std::string& value);

  // byte list
  Response get_list(const std::string& key);
  Response get_list(const std::string& key, const int index);
  Response get_list(const std::string& key, const int start_index, const int end_index);
  Response push_list(const std::string& key, const ValueType type, const std::string& value);

  struct Stats {
    size_t hits;
    size_t misses;
    size_t keys;
  };

  Stats get_stats() const;
};
