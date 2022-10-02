#include <iostream>
#include <vector>
#include <atomic>
#include <chrono>

#include "file_finder/command_runner.h"
#include "file_finder/seeker.h"
#include "file_finder/shared_queue.h"

namespace ff = file_finder;

constexpr long long periodic_dump_interval_ms = 200;
constexpr int max_args_allowed = 100; //TODO: Maybe pick a less arbitrary number?
std::atomic_flag g_exit_flag; // In C++20 this is initialized clearly to false.

void register_commands(ff::CommandRunner &runner)
{
    runner.register_command("dump", [](){
        ff::get_queue_instance().dump_queue();
    });

    runner.register_command("exit", []() {
        g_exit_flag.test_and_set();
    });
}

int main(int argc, char *argv[])
{
    if (max_args_allowed < argc - 2) {
        std::cerr << "Too many substrings to search, please try with " << max_args_allowed << " arguments or less.\n";
        return EXIT_FAILURE;
    }

    if (argc < 3) {
        std::cerr << "You need to provide a directory and at least one one substring to search. \n";
        return EXIT_FAILURE;
    }

    // Construct the command runner and register commands dump and exit.
    ff::CommandRunner runner;
    register_commands(runner);

    // Initialize the queue through its get function once before new threads are created.
    ff::get_queue_instance();

    // Construct the file seeker with the list of substrings and target directory, then start the search.
    const std::vector<std::string> args(argv + 2, argv + argc);
    ff::Seeker seeker{args, argv[1]};
    seeker.start_search();

    // Create a thread that will periodically dump the container contents to console.
    auto periodic_dump = []() {
        while (true) {
            if (g_exit_flag.test()) {
                std::cout << "Periodic dump exiting...\n";
                return;
            }
            ff::get_queue_instance().dump_queue();
            std::this_thread::sleep_for(std::chrono::milliseconds(periodic_dump_interval_ms));
        }
    };

    auto periodic_dump_ft = std::async(periodic_dump);

    // Start main loop to get user input
    std::string user_input{};
    while (true) {
        // Cleanup and exit if the exit flag is set.
        if (g_exit_flag.test()) {
            seeker.stop_search();
            periodic_dump_ft.get();
            std::cout << "Exiting program.\n";
            return EXIT_SUCCESS;
        }

        // Get command from user and run it.
        do {
            std::cin.clear();
            std::cin >> user_input;
        } while (!runner.command_exists(user_input));

        runner(user_input);
    }
}