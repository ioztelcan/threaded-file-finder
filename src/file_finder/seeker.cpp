#include <vector>
#include <filesystem>

#include <iostream>
#include <chrono>
#include <mutex>
//#include <syncstream>

#include "file_finder/seeker.h"
#include "file_finder/shared_queue.h"

namespace fs = std::filesystem;

namespace file_finder {

//TODO: Think about syncstream situation
std::mutex cout_mutex;

// Constructors
Seeker::Seeker(std::vector<std::string_view> substrings) : m_substrings{std::move(substrings)}
{ }

// Public
void Seeker::start_search(const std::string &directory)
{
    for (const auto &substring: m_substrings) {
        m_threads.push_back(std::async(&Seeker::search, this, substring, directory));
    }
}

// Private
void Seeker::search(const std::string_view &substr, const std::string &directory)
{
    SharedQueue &shared_queue = get_queue_instance();
    std::cout << "Addr of sq: " << &shared_queue << "\n";
    try {
        fs::current_path(directory);
    } catch (const std::exception &ex) {
        std::cerr << "Exception caught: " << ex.what() << "\n";
        // TODO: Better handling of these exit conditions.
        return;
    }

    for (const auto& dir_entry : fs::recursive_directory_iterator{directory})
    {
        if (!dir_entry.is_directory()) {
            if(std::string {dir_entry.path().filename() }.find(substr) != std::string::npos) {
                std::lock_guard<std::mutex> guard{cout_mutex};
                std::cout << dir_entry.path().filename() << "\n";
                // TODO: This has to actually write into the container
            }
        }
    }
}

} // namespace file_finder