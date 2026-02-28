#pragma once

#include "MCache.h"
#include <sstream>

class CLI {
public:
    CLI(MCache& cache);

    void run();

private:
    MCache& cache_;
    bool running_;

    using CommandHandler = std::function<void(const std::vector<std::string>&)>;
    std::unordered_map<std::string, CommandHandler> command_table_;

    // helpers
    std::vector<std::string> tokenize(const std::string& line);
    void dispatch(const std::vector<std::string>& tokens);

    // command handlers
    void handle_set(const std::vector<std::string>& args);
    void handle_add(const std::vector<std::string>& args);
    void handle_get(const std::vector<std::string>& args);
    void handle_del(const std::vector<std::string>& args);
    void handle_stat(const std::vector<std::string>& args);
    void handle_plist(const std::vector<std::string>& args);
    void handle_glist(const std::vector<std::string>& args);

    inline static const std::unordered_map<std::string, MCache::ValueType> types = {
        {"int", MCache::ValueType::INT},
        {"float", MCache::ValueType::FLOAT},
        {"string", MCache::ValueType::STRING}
    };
};
