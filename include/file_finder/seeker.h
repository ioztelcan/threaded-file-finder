#pragma once
#include <future>
#include <atomic>

namespace file_finder {

class Seeker {
private:
    const std::vector<std::string_view> m_substrings;
    std::vector<std::future<void>> m_workers;
    std::atomic_flag m_stop_workers;

    void search(const std::string_view &substr, const std::string &directory);

public:
    explicit Seeker(std::vector<std::string_view> substrings);
    void start_search(const std::string &directory);
    void stop_search();

};

} // namespace file_finder