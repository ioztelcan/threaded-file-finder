#pragma once

#include <unordered_map>
#include <functional>
#include <string>

namespace file_finder {

using command_t = std::function<void()>;

class CommandRunner {
private:
    /**
     * A hashmap to keep the commands and their respective callback functions.
     * command names are strings and command type is of std::function specialized for
     * functions that return void and don't take any arguments.
     */
    using command_list_t = std::unordered_map<std::string, command_t>;
    command_list_t m_commands;
public:
    /**
     * @brief Adds the commands and their callbacks to the unordered map of CommandRunner
     * class.
     *
     * @param command_name      std::string&, the command user can enter via console.
     * @param command_function  command_t&, the callback function to be ran after this command.
     */
    void register_command(const std::string &command_name, const command_t &command_function);

    /**
     * @brief A helper function to check whether a certain command exists in the map or not.
     *
     * @param command   std::string&, name of the command
     * @return          true if the command exists, false otherwise.
     */
    [[nodiscard]] bool command_exists(const std::string &command) const;

    /**
     * @brief Function call operator overload to run the command passed by the user. Calls the
     * function that corresponds to the passed command in the map.
     *
     * @param command std::string&, name of the command to be run.
     */
    void operator()(const std::string &command);
};

} // namespace file_finder
