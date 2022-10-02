#include <vector>
#include <filesystem>
#include <iostream>
#include <mutex>

#include <chrono>

#include "file_finder/seeker.h"
#include "file_finder/shared_queue.h"

namespace file_finder {

// Constructors
Seeker::Seeker(std::vector<std::string> substrings, std::string search_dir) : m_substrings{std::move(substrings)},
                                                                              m_search_dir{std::move(search_dir)}
{ }

// Public
void Seeker::start_search()
{
    for (const auto &substring: m_substrings) {
        m_workers.push_back(std::async(&Seeker::search, this, substring, m_search_dir));
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
void Seeker::search(const std::string &substr, const std::string &directory)
{
    if (m_stop_workers.test()) {
        std::cout << "Search halted...\n";
        return;
    }

    namespace fs = std::filesystem;
    SharedQueue &shared_queue = get_queue_instance();

    for (const auto& dir_entry : fs::recursive_directory_iterator{directory})
    {
        if (m_stop_workers.test()) {
            std::cout << "Search halted...\n";
            return;
        }
        if (!dir_entry.is_directory()) {
            if(std::string {dir_entry.path().filename()}.find(substr) != std::string::npos) {
                shared_queue.push(std::string{dir_entry.path().filename()});
            }
        }
        //std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

} // namespace file_finder