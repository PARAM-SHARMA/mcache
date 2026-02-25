#include "cli.h"
#include <iostream>
#include <variant>

void CLI::run() {
  enum class Command {
    GET,
    SET,
    ADD,
    DEL,
    STAT,
    PLIST,
    GLIST,
    EXIT,
    UNKNOWN
  };

  static const std::unordered_map<std::string, Command> commands = {
    {"get", Command::GET},
    {"set", Command::SET},
    {"add", Command::ADD},
    {"del", Command::DEL},
    {"stat", Command::STAT},
    {"plist", Command::PLIST},
    {"glist", Command::GLIST},
    {"exit", Command::EXIT},
  };

  std::string cmd, key;
  std::string type;
  std::string value;

  while (true) {
    std::cout << "MCache > ";
    std::cin >> cmd;

    Command command;
    auto it = commands.find(cmd);
    if (it != commands.end()) {
      command = it->second;
    } else {
      command = Command::UNKNOWN;
    }

    switch (command) {

      case Command::SET: {
        std::cin >> type >> key;
        std::cin.ignore();
        std::getline(std::cin, value);
        bool success = cache_.set_val(key, type, value);

        if (success) {
          std::cout << "true " << value << std::endl;
        } else {
          std::cout << "false" << std::endl;
        }
        break;
      }

      case Command::ADD: {
        std::cin >> type >> key;
        std::cin.ignore();
        std::getline(std::cin, value);
        bool success = cache_.add_val(key, type, value);

        if (success) {
          std::cout << "true " << value << std::endl;
        } else {
          std::cout << "false" << std::endl;
        }
        break;
      }

      case Command::DEL: {
        std::cin >> key;
        if (cache_.del_val(key)) {
          std::cout << "true" << std::endl;
        } else {
          std::cout << "false" << std::endl;
        }

        break;
      }

      case Command::GET: {
        std::cin >> key;
        auto val = cache_.get_val(key);

        if (val.has_value()) {
          std::cout << "true {" << val->first << "} " << val->second << std::endl;
        } else {
          std::cout << "false" << std::endl;
        }
        break;
      }

      case Command::STAT: {
        MCache::Stats stat = cache_.get_stats();

        std::cout << "hits -> " << stat.hits << std::endl;
        std::cout << "misses -> " << stat.misses << std::endl;
        std::cout << "keys -> " << stat.keys << std::endl;
        break;
      }

      case Command::PLIST: {
        std::cin >> type >> key;
        std::cin.ignore();
        std::getline(std::cin, value);

        bool success = cache_.push_list(key, type, value);

        if (success) {
          std::cout << "true" << std::endl;
        } else {
          std::cout << "false" << std::endl;
        }
        break;
      }

      case Command::GLIST: {
        std::cin >> key;
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
        break;
      }

      case Command::EXIT:
        return;

      case Command::UNKNOWN:
        std::cout << "Unknown command" << std::endl;
        break;
    }
  }
  return;
}

