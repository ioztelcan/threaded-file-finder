#include <iostream>
#include <vector>
#include <atomic>
#include <chrono>

#include "file_finder/seeker.h"
#include "file_finder/shared_queue.h"

// command'lari handle eden thread de keep_working = false dedikten sonra tum threadleri join edecek

namespace ff = file_finder;

constexpr int max_args_allowed = 100;
std::atomic_flag exit_flag; // In C++20 this is initialized clearly to false.

int main(int argc, char * argv[])
{
    if (max_args_allowed < argc - 2) {
        std::cerr << "Too many substrings to search, please try with " << max_args_allowed << " arguments or less.\n";
        return EXIT_FAILURE;
    }

    if (argc < 3) {
        std::cerr << "You need to provide a directory and at least one one substring to search. \n";
        return EXIT_FAILURE;
    }

    ff::SharedQueue &shared_queue = ff::get_queue_instance();
    std::string search_dir {argv[1]};
    const std::vector<std::string_view> args(argv + 2, argv + argc);

    ff::Seeker seeker{args};
    seeker.start_search(search_dir);

    while (true) {
        shared_queue.dump_queue();
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
}