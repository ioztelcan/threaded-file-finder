#include <iostream>

#include "file_finder/shared_queue.h"

namespace file_finder {

// Public functions
void SharedQueue::push(const std::string &file)
{
    std::lock_guard guard{m_access_queue};
    m_queue.push(file);
}

std::string SharedQueue::pop()
{
    std::lock_guard guard{m_access_queue};
    auto file = m_queue.front();
    m_queue.pop();
    return file;
}

void SharedQueue::dump_queue()
{
    // Swap first with an empty one, so lock is held much less.
    std::queue<std::string> dump{};
    {
        std::lock_guard guard{m_access_queue};
        std::swap(m_queue, dump);
    }
    while (!dump.empty()) {
        std::cout << dump.front() << "\n";
        dump.pop();
    }
}

// Non-member functions
SharedQueue &get_queue_instance()
{
    static SharedQueue shared_queue;
    return shared_queue;
}

} // namespace file_finder