#pragma once
#include <future>
#include <atomic>

namespace file_finder {

class Seeker {
private:
    /**
     * Data members of seeker class to hold the search directory and the substrings to be searched.
     */
    const std::vector<std::string> m_substrings;
    const std::string m_search_dir;
    /**
     * A vector of futures that will represent the spawned seekers for each substring.
     */
    std::vector<std::future<void>> m_workers;
    /**
     * An atomic flag to signal that the program is terminating and clean-up should begin.
     */
    std::atomic_flag m_stop_workers;

    /**
     * @brief Search function that recursively goes through the provided directory to find the
     * filenames that contain the substring being searched for. Any filename that contains the
     * substring is added to the shared container (SharedQueue object).
     *
     * @param substr    std::string&, The substring to be found in filenames
     * @param directory std::string&, Directory to be searched recursively.
     */
    void search(const std::string &substr, const std::string &directory);
public:
    /**
     * @brief Constructor that builds the Seeker class with the list of substring to be searched
     * for and the search directory.
     *
     * @param substrings std::vector<std::string>, Substrings to be searched for.
     * @param search_dir std::string, search directory
     */
    Seeker(std::vector<std::string> substrings, std::string search_dir);

    /**
     * @brief Spawns search threads for each substring with std::async and adds their futures to m_workers.
     */
    void start_search();

    /**
     * @brief Sets the atomic m_stop_workers flag and joins the worker threads.
     */
    void stop_search();
};

} // namespace file_finder