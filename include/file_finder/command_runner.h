#pragma once
#include <unordered_map>
#include <functional>
#include <string>

namespace file_finder {

using command_t = std::function<void()>;

class CommandRunner {
public:
    void register_command(const std::string &command_name, const command_t &command_function);

    void operator()(const std::string &command);

    [[nodiscard]] bool command_exists(const std::string &command) const;
private:
    using command_list_t = std::unordered_map<std::string, command_t>;
    command_list_t m_commands;
};

} // namespace file_finder
