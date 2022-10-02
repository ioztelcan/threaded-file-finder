
#include "file_finder/command_runner.h"

namespace file_finder {

void CommandRunner::register_command(const std::string &command_name, const command_t &command_function)
{
    m_commands[command_name] = command_function;
}

void CommandRunner::operator()(const std::string &command)
{
    return m_commands[command]();
}

bool CommandRunner::command_exists(const std::string &command) const
{
    return m_commands.contains(command);
}

} // namespace file_finder