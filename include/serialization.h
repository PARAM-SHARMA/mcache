#pragma once

#include <vector>
#include <cstdint>
#include <string>
#include <cstring>
#include <type_traits>

namespace serialization {
enum class InferredType { INT, FLOAT, STRING };

template <typename T>
std::vector<uint8_t> to_bytes(const T& obj) {
  static_assert(std::is_trivially_copyable_v<T>, "Type should be trivially copyable");
  std::vector<uint8_t> data(sizeof(T));
  std::memcpy(data.data(), &obj, sizeof(T));
  return data;
}

template <typename T>
T from_bytes(const std::vector<uint8_t>& data) {
  static_assert(std::is_trivially_copyable_v<T>, "Type should be trivially copyable");
  if (data.size() != sizeof(T)) {
    throw std::runtime_error("Invalid data size for conversion");
  }
  T obj;
  std::memcpy(&obj, data.data(), sizeof(T));
  return obj;
}

inline std::vector<uint8_t> to_bytes(const std::string& str) {
  return std::vector<uint8_t>(str.begin(), str.end());
}

inline std::string from_bytes_string(const std::vector<uint8_t>& data) {
  return std::string(data.begin(), data.end());
}

}
