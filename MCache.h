#ifndef MC_H
#define MC_H

#include <unordered_map>
#include <string>
#include <optional>
#include <mutex>

class MCache {
private:
  std::unordered_map<std::string, int> map;
  std::mutex mtx;
  
public:
  int hits;
  int misses;
  int keys;

  MCache();

  std::optional<int> get_val(const std::string& key);

  bool add(const std::string& key, const int value);

  bool set_val(const std::string& key, const int value);

  bool del_val(const std::string& key);

  std::unordered_map<std::string, int> stats();
};


#endif // MC_H
