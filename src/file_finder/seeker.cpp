#include <vector>
#include <filesystem>
#include <iostream>
#include <mutex>
//#include <chrono>

#include "file_finder/seeker.h"
#include "file_finder/shared_queue.h"

namespace file_finder {

// Private functions
void Seeker::search(const std::string &substr, const std::string &directory)
{
    if (m_stop_workers.test()) {
        std::cout << "Substring search halted.\n";
        return;
    }

    auto &g_shared_queue = get_queue_instance();
    namespace fs = std::filesystem;

    //TODO: Doing a BFS over dirs rather than recursive search can enable using threads for each dir, speeding up the search and not blowing the stack.
    try {
        // Iterate over directories and files recursively.
        for (const auto& dir_entry : fs::recursive_directory_iterator{directory})
        {
            if (m_stop_workers.test()) {
                std::cout << "Substring search halted.\n";
                return;
            }
            // If the entry is a file, search for the substring in the filename, if found add it to the shared container.
            if (!dir_entry.is_directory()) {
                if (std::string{dir_entry.path().filename()}.find(substr) != std::string::npos) {
                    g_shared_queue.push(std::string{dir_entry.path().filename()});
                }
            }
            // To be able to test concurrent behaviour easily, following delay can be uncommented along with chrono library.
            //std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    } catch (const std::exception &ex) {
        std::cerr << "Exception caught: " << ex.what() << "\n";
    }
}

// Public functions
Seeker::Seeker(std::vector<std::string> substrings, std::string search_dir) : m_substrings{std::move(substrings)},
                                                                              m_search_dir{std::move(search_dir)}
{ }

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

} // namespace file_finder