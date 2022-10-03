#include "file_finder/command_runner.h"

namespace file_finder {

// Public functions
void CommandRunner::register_command(const std::string &command_name, const command_t &command_function)
{
    m_commands[command_name] = command_function;
}

bool CommandRunner::command_exists(const std::string &command) const
{
    return m_commands.contains(command);
}

void CommandRunner::operator()(const std::string &command)
{
    return m_commands[command]();
}

} // namespace file_finder