#include "MCache.h"
#include "serialization.h"

MCache::Response MCache::get_list(const std::string& key) {
  std::lock_guard<std::mutex> lock(mtx_);
  auto it = store_.find(key);

  if (it == store_.end()) {
    ++misses_;
    return Response{false, "", "", std::nullopt, "Key not found"};
  }

  ++hits_;
  const CacheValue& cv = it->second;

  if (cv.s_type == StructType::LIST) {
    return std::visit([&](auto& container) -> Response {
      if constexpr (std::is_same_v<std::decay_t<decltype(container)>, ByteList>) {
        if (cv.type == ValueType::INT) {
          std::vector<int> deserialized_list;
          for (const auto& raw_bytes : container) {
            int value = serialization::from_bytes<int>(raw_bytes);
            deserialized_list.push_back(value);
          }
          return Response{true, "list", "int", deserialized_list, ""};
        }
        else if (cv.type == ValueType::FLOAT) {
          std::vector<float> deserialized_list;
          for (const auto& raw_bytes : container) {
            float value = serialization::from_bytes<float>(raw_bytes);
            deserialized_list.push_back(value);
          }
          return Response{true, "list", "float", deserialized_list, ""};
        }
        else if (cv.type == ValueType::STRING) {
          std::vector<std::string> deserialized_list;
          for (const auto& raw_bytes : container) {
            std::string value = serialization::from_bytes_string(raw_bytes);
            deserialized_list.push_back(value);
          }
          return Response{true, "list", "string", deserialized_list, ""};
        }
        else {
          return Response{false, "", "", std::nullopt, "Unsupported type in list"};
        }
      }
      return Response{false, "", "", std::nullopt, "Unexpected type"};
    }, cv.data);
  }

  return Response{false, "", "", std::nullopt, "Not a LIST type"};
}

MCache::Response MCache::push_list(const std::string& key, const MCache::ValueType type, const std::string& values) {
  std::lock_guard<std::mutex> lock(mtx_);
  MCache::ByteList bl;
  std::string new_type;

  serialization::construct_list(type, bl, values);

  auto it = store_.find(key);
  if (it != store_.end()) {
    if (auto* existing_list = std::get_if<ByteList>(&it->second.data)) {
      if (it->second.type == type) {
        existing_list->insert(existing_list->end(), bl.begin(), bl.end());
      } else {
        return Response{false, "", "", std::nullopt, "Unexpected type"};
      }
    } else {
      return Response{false, "", "", std::nullopt, "Unexpected type"};
    }

  } else {
    CacheValue cv;

    cv.type = type;
    if (type == MCache::ValueType::INT) {
      new_type = "int";
    } else if (type == MCache::ValueType::FLOAT) {
      new_type = "float";
    } else if (type == MCache::ValueType::STRING) {
      new_type = "string";
    }

    cv.s_type = MCache::StructType::LIST;
    cv.data = std::move(bl);
    store_[key] = std::move(cv);
  }

  return Response{true, "list", new_type, std::nullopt, ""};
}
