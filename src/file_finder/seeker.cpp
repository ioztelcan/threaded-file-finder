#include <vector>
#include <filesystem>
#include <iostream>
#include <mutex>

#include <chrono>

#include "file_finder/seeker.h"
#include "file_finder/shared_queue.h"

namespace fs = std::filesystem;

namespace file_finder {


// Constructors
Seeker::Seeker(std::vector<std::string_view> substrings) : m_substrings{std::move(substrings)}
{ }

// Public
void Seeker::start_search(const std::string &directory)
{
    for (const auto &substring: m_substrings) {
        m_workers.push_back(std::async(&Seeker::search, this, substring, directory));
    }
}

void Seeker::stop_search()
{
    m_stop_workers.test_and_set();
    for (auto &worker_ft: m_workers) {
        worker_ft.get();
    }
}

// Private
void Seeker::search(const std::string_view &substr, const std::string &directory)
{
    if (m_stop_workers.test()) {
        std::cout << "Search halted...\n";
        return;
    }
    SharedQueue &shared_queue = get_queue_instance();

    try {
        fs::current_path(directory);
    } catch (const std::exception &ex) {
        std::cerr << "Exception caught: " << ex.what() << "\n";
        // TODO: Better handling of these exit conditions.
        return;
    }

    for (const auto& dir_entry : fs::recursive_directory_iterator{directory})
    {
        if (m_stop_workers.test()) {
            std::cout << "Search halted...\n";
            return;
        }
        if (!dir_entry.is_directory()) {
            if(std::string {dir_entry.path().filename() }.find(substr) != std::string::npos) {
                shared_queue.push(std::string{dir_entry.path().filename()});
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

} // namespace file_finder