#pragma once
#include <queue>
#include <string>
#include <mutex>

namespace file_finder {

class SharedQueue {
private:
    // TODO: If it remains like this, think about reserving a certain amount of space to avoid reallocation.
    std::queue<std::string> m_files_found;
    std::mutex m_access_queue;
public:
    //SharedQueue();

    void push(const std::string &file);

    std::string pop();

    void dump_queue();
};

SharedQueue &get_queue_instance();

} // namespace file_finder

