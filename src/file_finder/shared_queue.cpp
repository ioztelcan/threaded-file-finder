#include <iostream>

#include "file_finder/shared_queue.h"

namespace file_finder {

void SharedQueue::push(const std::string &file)
{
    std::lock_guard<std::mutex> guard{m_access_queue};
    m_files_found.push(file);
}

std::string SharedQueue::pop()
{
    std::lock_guard<std::mutex> guard{m_access_queue};
    auto file = m_files_found.front();
    m_files_found.pop();
    return file;
}

void SharedQueue::dump_queue()
{
    std::lock_guard<std::mutex> guard{m_access_queue};
    while (!m_files_found.empty()) {
        std::cout << m_files_found.front() << "\n";
        m_files_found.pop();
    }
}

SharedQueue &get_queue_instance()
{
    static SharedQueue shared_queue;
    return shared_queue;
}

} // namespace file_finder