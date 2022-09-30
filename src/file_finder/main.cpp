#include <iostream>
#include <vector>
#include <atomic>
#include <chrono>

#include "file_finder/seeker.h"
#include "file_finder/shared_queue.h"
//#include "file_finder/commands.h"

namespace ff = file_finder;

constexpr int max_args_allowed = 100; //TODO: Maybe pick a less arbitrary number?
std::atomic_flag g_exit_flag; // In C++20 this is initialized clearly to false.

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

    const std::vector<std::string> args(argv + 2, argv + argc);

    ff::Seeker seeker{args, argv[1]};
    seeker.start_search();

    ff::SharedQueue &files_found = ff::get_queue_instance();

    auto periodic_dump = [&files_found]() {
        while (true) {
            if (g_exit_flag.test()) {
                std::cout << "Periodic dump exiting...\n";
                return;
            }
            files_found.dump_queue();
            std::this_thread::sleep_for(std::chrono::seconds(5));
        }
    };

    auto periodic_dump_ft = std::async(periodic_dump);

    std::string user_input;
    while (true) {
        do {
            std::cin.clear();
            std::cin >> user_input;
        } while ((user_input != "dump") && (user_input != "exit"));
        
        if (user_input == "dump") {
            std::cout << "dumping...\n";
            files_found.dump_queue();
        } else {
            g_exit_flag.test_and_set();
            seeker.stop_search();
            periodic_dump_ft.get();
            std::cout << "Exiting program.\n";
            return EXIT_SUCCESS;
        }
    }
}