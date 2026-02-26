#include "MCache.h"
#include "serialization.h"

MCache::Response MCache::get_val(const std::string& key) {
  std::lock_guard<std::mutex> lock(mtx_);

  auto it = store_.find(key);
  if (it == store_.end()) {
    ++misses_;
    return Response{false, "", "", std::nullopt, "Key not found"};
  }

  ++hits_;
  const CacheValue& cv = it->second;
  MCache::Response result = Response{true, "", "", std::nullopt, ""};

  // First dispatch by structure type
  switch (cv.s_type) {
    case StructType::RAW: {
      std::visit([&](auto& raw_bytes) {
        using T = std::decay_t<decltype(raw_bytes)>;
        if constexpr (std::is_same_v<T, Bytes>) {
          switch (cv.type) {
            case ValueType::INT: {
              int v = serialization::from_bytes<int>(raw_bytes);
              result.type = "int";
              result.data = v;
              break;
            }
            case ValueType::FLOAT: {
              float v = serialization::from_bytes<float>(raw_bytes);
              result.type = "float";
              result.data = v;
              break;
            }
            case ValueType::STRING: {
              std::string v = serialization::from_bytes_string(raw_bytes);
              result.type = "string";
              result.data = v;
              break;
            }
          }
        }
      }, cv.data);
      break;
    }

    case StructType::LIST: {
      std::visit([&](auto& list){
        using T = std::decay_t<decltype(list)>;
        if constexpr (std::is_same_v<T, ByteList>) {
          result.type = "list";
          result.data = "size=" + std::to_string(list.size());
        }
      }, cv.data);
      break;
    }

    case StructType::SET: {
      std::visit([&](auto& s){
        using T = std::decay_t<decltype(s)>;
        if constexpr (std::is_same_v<T, ByteSet>) {
          result.type = "set";
          result.data = "size=" + std::to_string(s.size());
        }
      }, cv.data);
      break;
    }

    case StructType::MAP: {
      std::visit([&](auto& m){
        using T = std::decay_t<decltype(m)>;
        if constexpr (std::is_same_v<T, ByteMap>) {
          result.type = "map";
          result.data = "size=" + std::to_string(m.size());
        }
      }, cv.data);
      break;
    }
  }

  return result;
}

MCache::Response MCache::add_val(const std::string& key, const std::string& type, const std::string& value) {
  std::lock_guard<std::mutex> lock(mtx_);
  if (store_.find(key) != store_.end())
    return Response{false, "", "", std::nullopt, "Key already exists"};

  if (auto parsed = parse_value(type, value)) {
    store_.emplace(key, *parsed);
    return Response{true, "raw", type, std::nullopt, ""};
  }

  return Response{false, "", "", std::nullopt, "Unexpected Type"};
}

MCache::Response MCache::set_val(const std::string& key, const std::string& type, const std::string& value) {
  std::lock_guard<std::mutex> lock(mtx_);
  auto it = store_.find(key);
  if (it == store_.end()) {
    return Response{false, "", "", std::nullopt, "Key not found"};
  }

  if (auto parsed = parse_value(type, value)) {
    it->second = *parsed;
    return Response{true, "raw", type, std::nullopt, ""};
  } else {
    return Response{false, "", "", std::nullopt, "Unexpected Type"};
  }
}

bool MCache::del_key(const std::string& key) noexcept {
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

std::optional<MCache::CacheValue> MCache::parse_value(const std::string& type, const std::string& value) {
  MCache::CacheValue c_val;
  try {
    c_val.s_type = StructType::RAW;
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
      return std::nullopt;
    }

  } catch (const std::invalid_argument&) {
    return std::nullopt;
  } catch (const std::out_of_range&) {
    return std::nullopt;
  }
  return c_val;
}
