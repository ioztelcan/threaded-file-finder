#pragma once
#include <unordered_map>
#include <functional>

namespace file_finder {

enum class command_result {Success, Failure, Exit};
using command_t = std::function<command_result()>;
using command_list_t = std::unordered_map<std::string, command_t>;

class CommandRunner {
public:
    void register_command(const std::string &command_name, const command_t &command_function)
    {
        m_commands[command_name] = command_function;
    }

    command_result operator()(const std::string &command)
    {
        return m_commands[command]();
    }
private:
    using command_list_t = std::unordered_map<std::string, std::function<command_result()>>;
    command_list_t m_commands;
};


//class Command

}
