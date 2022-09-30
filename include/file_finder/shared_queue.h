#pragma once
#include <queue>
#include <string>
#include <mutex>

namespace file_finder {

class SharedQueue {
private:
    std::queue<std::string> m_queue;
    std::mutex m_access_queue;
public:
    void push(const std::string &file);
    std::string pop();
    void dump_queue();
};

SharedQueue &get_queue_instance();

} // namespace file_finder

