#pragma once
#include <future>
#include <atomic>

namespace file_finder {

class Seeker {
private:
    const std::vector<std::string> m_substrings;
    const std::string m_search_dir;
    std::vector<std::future<void>> m_workers;
    std::atomic_flag m_stop_workers;
    void search(const std::string &substr, const std::string &directory);
public:
    Seeker(std::vector<std::string> substrings, std::string search_dir);
    void start_search();
    void stop_search();
};

} // namespace file_finder