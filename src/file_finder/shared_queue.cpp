#include <iostream>

#include "file_finder/shared_queue.h"

namespace file_finder {

//Public
void SharedQueue::push(const std::string &file)
{
    std::lock_guard<std::mutex> guard{m_access_queue};
    m_queue.push(file);
}

std::string SharedQueue::pop()
{
    std::lock_guard<std::mutex> guard{m_access_queue};
    auto file = m_queue.front();
    m_queue.pop();
    return file;
}

void SharedQueue::dump_queue()
{
    std::lock_guard<std::mutex> guard{m_access_queue};
    while (!m_queue.empty()) {
        std::cout << m_queue.front() << "\n";
        m_queue.pop();
    }
}

// Other
SharedQueue &get_queue_instance()
{
    static SharedQueue shared_queue;
    return shared_queue;
}

} // namespace file_finder