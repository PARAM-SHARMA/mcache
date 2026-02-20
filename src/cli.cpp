#include "cli.h"
#include <iostream>

void CLI::run() {
  enum class Command {
    GET,
    SET,
    ADD,
    DEL,
    STAT,
    EXIT,
    UNKNOWN
  };

  static const std::unordered_map<std::string, Command> commands = {
    {"get", Command::GET},
    {"set", Command::SET},
    {"add", Command::ADD},
    {"del", Command::DEL},
    {"stat", Command::STAT},
    {"exit", Command::EXIT},
  };

  std::string cmd, key;
  int value;

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
        std::cin >> key >> value;
        bool success = cache_.set_val(key, value);

        if (success) {
          std::cout << "true " << value << std::endl;
        } else {
          std::cout << "false" << std::endl;
        }
        break;
      }

      case Command::ADD: {
        std::cin >> key >> value;
        bool success = cache_.add_val(key, value);

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

        if (val) {
          std::cout << "true " << *val << std::endl;
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

      case Command::EXIT:
        return;

      case Command::UNKNOWN:
        std::cout << "Unknown command" << std::endl;
        break;
    }
  }
  return;
}

