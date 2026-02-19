#include "MCache.h"
#include <iostream>
#include <string>
#include <unordered_map>

enum Command {
  GET,
  SET,
  ADD,
  DEL,
  STAT,
  EXIT,
  UNKNOWN
};

Command getCommand(const std::string& cmd) {
  if (cmd == "get") return GET;
  if (cmd == "set") return SET;
  if (cmd == "add") return ADD;
  if (cmd == "del") return DEL;
  if (cmd == "stat") return STAT;
  if (cmd == "exit") return EXIT;
  return UNKNOWN;
}

int main() {
  MCache mc;
  std::string cmd, key;
  int value;

  while (true) {
    std::cout << "> ";
    std::cin >> cmd;

    Command command = getCommand(cmd);

    switch (command) {

      case SET: {
        std::cin >> key >> value;
        mc.set_val(key, value);

        auto val = mc.get_val(key);
        if (val) {
          std::cout << "true " << *val << std::endl;
        } else {
          std::cout << "false" << std::endl;
        }
        break;
      }

      case ADD: {
        std::cin >> key >> value;
        mc.add(key, value);

        auto val = mc.get_val(key);
        if (val) {
          std::cout << "true " << *val << std::endl;
        } else {
          std::cout << "false" << std::endl;
        }
        break;
      }

      case DEL: {
        std::cin >> key;
        if (mc.del_val(key)) {
          std::cout << "true" << std::endl;
        } else {
          std::cout << "false" << std::endl;
        }

        break;
      }

      case GET: {
        std::cin >> key;
        auto val = mc.get_val(key);

        if (val) {
          std::cout << "true " << *val << std::endl;
        } else {
          std::cout << "false" << std::endl;
        }
        break;
      }

      case STAT: {
        std::unordered_map<std::string, int> stat = mc.stats();

        for (const auto& it : stat) {
          std::cout << it.first << " -> " << it.second << std::endl;
        }
        break;
      }

      case EXIT:
        return 0;

      case UNKNOWN:
        std::cout << "Unknown command" << std::endl;
        break;
    }
  }

  return 0;
}
