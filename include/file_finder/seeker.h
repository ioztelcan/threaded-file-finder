#pragma once
#include <future>

namespace file_finder {

class Seeker {
private:
    std::vector<std::string_view> m_substrings;
    std::vector<std::future<void>> m_threads;

    void search(const std::string_view &substr, const std::string &directory);

public:
    explicit Seeker(std::vector<std::string_view> substrings);
    void start_search(const std::string &directory);

};

} // namespace file_finder