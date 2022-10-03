#pragma once
#include <queue>
#include <string>
#include <mutex>

namespace file_finder {

class SharedQueue {
private:
    /**
     * Underlying data structure for the shared queue is a std::queue, holding strings
     * for each filename that contains the substrings.
     */
    std::queue<std::string> m_queue;
    // A mutex to regulate access to the queue.
    std::mutex m_access_queue;
public:
    /**
     * @brief Function to push a new filename to the end of the queue.
     *
     * @param file std::string&, File to be pushed in the queue.
     */
    void push(const std::string &file);

    /**
     * @brief Function to get the value in front of the queue and remove it from the queue.
     *
     * @return std::string, filename at the front of the queue.
     */
    std::string pop();

    /**
     * @brief Writes all the filenames inside the queue to the console and removes them
     * from the queue.
     */
    void dump_queue();
};

/**
 * @brief Helper function to get the instance of the SharedQueue object used globally. Can
 * be used to lazy initialize the queue object as well.
 *
 * @return SharedQueue&, to the global shared queue object.
 */
SharedQueue &get_queue_instance();

} // namespace file_finder

