#include "cli.h"
#include <iostream>

CLI::CLI(MCache& cache) 
: cache_(cache), running_(true){
  command_table_ = {
    {"set",   [this](const auto& args){ handle_set(args); }},
    {"get",   [this](const auto& args){ handle_get(args); }},
    {"add",   [this](const auto& args){ handle_add(args); }},
    {"del",   [this](const auto& args){ handle_del(args); }},
    {"stat",  [this](const auto& args){ handle_stat(args); }},
    {"plist", [this](const auto& args){ handle_plist(args); }},
    {"glist", [this](const auto& args){ handle_glist(args); }},
    {"exit",  [this](const auto&){ running_ = false; }}
  };
}

std::vector<std::string> CLI::tokenize(const std::string& line) {
  std::istringstream iss(line);
  std::vector<std::string> tokens;
  std::string token;

  while(iss >> token)
    tokens.push_back(token);

  return tokens;
}

void CLI::dispatch(const std::vector<std::string>& tokens) {
  std::string cmd = tokens[0];
  for (char& c : cmd) {
    c = std::tolower(static_cast<unsigned char>(c));
  }

  auto it = CLI::command_table_.find(cmd);
  if (it == CLI::command_table_.end()) {
    std::cout << "Unknown command\n";
    return;
  }

  it->second(tokens);
}

void CLI::run() {

  running_ = true;
  while (running_) {
    std::cout << "MCache > ";

    std::string line;
    if (!std::getline(std::cin, line))
      break;

    auto tokens = tokenize(line);
    if (tokens.empty()) continue;

    dispatch(tokens);
  }
  return;
}

void CLI::handle_set(const std::vector<std::string>& args) {
  if (args.size() < 4) {
    std::cout << "Usage: set <type> <key> <value>\n";
    return;
  }

  const std::string& type = args[1];
  const std::string& key = args[2];

  std::string value;
  for (size_t i=3; i<args.size(); i++) {
    value += args[i];
    if (i+1 < args.size()) value += " ";
  }

  MCache::ValueType value_type;
  auto type_it = types.find(type);
  if (type_it != types.end()) {
    value_type = type_it->second;
  } else {
    std::cout << "false Invalid Type" << std::endl;
    return;
  }
  MCache::Response val = cache_.set_val(key, value_type, value);

  if (val.success) {
    std::cout << "true " << value << std::endl;
  } else {
    std::cout << "false " << val.error << std::endl;
  }
  return;
}

void CLI::handle_add(const std::vector<std::string>& args) {
  if (args.size() < 4) {
    std::cout << "Usage: add <type> <key> <value>\n";
    return;
  }

  const std::string& type = args[1];
  const std::string& key = args[2];

  std::string value;
  for (size_t i=3; i<args.size(); i++) {
    value += args[i];
    if (i+1 < args.size()) value += " ";
  }

  MCache::ValueType value_type;
  auto type_it = types.find(type);
  if (type_it != types.end()) {
    value_type = type_it->second;
  } else {
    std::cout << "false Invalid Type" << std::endl;
    return;
  }

  MCache::Response val = cache_.add_val(key, value_type, value);

  if (val.success) {
    std::cout << "true " << value << std::endl;
  } else {
    std::cout << "false " << val.error << std::endl;
  }
  return;
}

void CLI::handle_get(const std::vector<std::string>& args) {
  if (args.size() < 2) {
    std::cout << "Usage: get <key>\n";
    return;
  }

  const std::string& key = args[1];

  MCache::Response val = cache_.get_val(key);

  if (val.success) {
    std::cout << "true {" << val.type << "} ";

    if (val.data) {
      std::visit([](auto& v) {
        using T = std::decay_t<decltype(v)>;
        if constexpr (std::is_same_v<T, int>) {
          std::cout << v;
        } else if constexpr (std::is_same_v<T, float>) {
          std::cout << v;
        } else if constexpr (std::is_same_v<T, std::string>) {
          std::cout << v;
        } 
        std::cout << std::endl;
      }, *val.data);
    } else {
      std::cout << "Error: No data available in response." << std::endl;
    }
  } else {
    std::cout << "false " << val.error << std::endl;
  }
}

void CLI::handle_stat(const std::vector<std::string>& args) {
  MCache::Stats stat = cache_.get_stats();

  std::cout << "hits -> " << stat.hits << std::endl;
  std::cout << "misses -> " << stat.misses << std::endl;
  std::cout << "keys -> " << stat.keys << std::endl;
}

void CLI::handle_del(const std::vector<std::string>& args) {
  if (args.size() < 2) {
    std::cout << "Usage: del <key>\n";
    return;
  }

  const std::string& key = args[1];

  if (cache_.del_key(key)) {
    std::cout << "true" << std::endl;
  } else {
    std::cout << "false" << std::endl;
  }
}

void CLI::handle_plist(const std::vector<std::string>& args) {
  if (args.size() < 4) {
    std::cout << "Usage: plist <type> <key> <value>\n";
    return;
  }

  const std::string& type = args[1];
  const std::string& key = args[2];

  std::string value;
  for (size_t i=3; i<args.size(); i++) {
    value += args[i];
    if (i+1 < args.size()) value += " ";
  }

  MCache::ValueType value_type;
  auto type_it = types.find(type);
  if (type_it != types.end()) {
    value_type = type_it->second;
  } else {
    std::cout << "false Invalid Type" << std::endl;
    return;
  }

  MCache::Response val = cache_.push_list(key, value_type, value);

  if (val.success) {
    std::cout << "true" << std::endl;
  } else {
    std::cout << "false " << val.error << std::endl;
  }
  return;
}

void CLI::handle_glist(const std::vector<std::string>& args) {
  if (args.size() < 2) {
    std::cout << "Usage: glist <key>\n";
    return;
  }

  const std::string& key = args[1];

  MCache::Response val = cache_.get_list(key);

  if (val.success) {
    std::cout << "true {" << val.type << "} ";

    if (val.data) {
      std::visit([](auto& container) {
        using T = std::decay_t<decltype(container)>;
        if constexpr (std::is_same_v<T, std::vector<int>>) {
          std::cout << "List of ints: ";
          for (const auto& item : container) {
            std::cout << item << " ";
          }
        } else if constexpr (std::is_same_v<T, std::vector<float>>) {
          std::cout << "List of floats: ";
          for (const auto& item : container) {
            std::cout << item << " ";
          }
        } else if constexpr (std::is_same_v<T, std::vector<std::string>>) {
          std::cout << "List of strings: ";
          for (const auto& item : container) {
            std::cout << item << " ";
          }
        }
        std::cout << std::endl;
      }, *val.data);
    } else {
      std::cout << "Error: No data available in response." << std::endl;
    }
  } else {
    std::cout << "false " << val.error << std::endl;
  }
}
