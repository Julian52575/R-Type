/**
 * @file tsqueue.h
 * @brief Implementation of a thread-safe queue.
 */

#pragma once

#include <mutex>
#include <deque>
#include <iostream>
#include <optional>

/**
 * @class tsqueue
 * @brief Template class for a thread-safe queue.
 *
 * This class uses a mutex to ensure that operations on the queue
 * are safe in a multithreaded environment.
 *
 * @tparam T Type of the elements stored in the queue.
 */
template <typename T>
class tsqueue {
public:
    /**
     * @brief Default constructor.
     */
    tsqueue() = default;

    /**
     * @brief Deleted copy constructor.
     *
     * This class is non-copyable to avoid concurrency issues.
     */
    tsqueue(const tsqueue&) = delete;

    /**
     * @brief Deleted copy assignment operator.
     */
    tsqueue& operator=(const tsqueue&) = delete;

    /**
     * @brief Returns the current size of the queue.
     * 
     * @return Number of elements in the queue.
     */
    uint32_t size() {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_queue.size();
    }

    /**
     * @brief Checks if the queue is empty.
     * 
     * @return `true` if the queue is empty, `false` otherwise.
     */
    bool empty() {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_queue.empty();
    }

    /**
     * @brief Clears all elements in the queue.
     */
    void clear() {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_queue.clear();
    }

    /**
     * @brief Retrieves and removes the first element from the queue.
     *
     * @return A `std::optional` containing the front element, or `std::nullopt` if the queue is empty.
     */
    std::optional<T> pop() {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_queue.empty())
            return std::nullopt;
        T item = m_queue.front();
        m_queue.pop_front();
        return std::make_optional(item);
    }

    /**
     * @brief Adds a new element to the end of the queue.
     * 
     * @param item The element to add.
     */
    void push(const T& item) {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_queue.push_back(item);
    }

    /**
     * @brief Destructor. Cleans up the queue before destroying the object.
     */
    ~tsqueue() {
        clear();
    }

private:
    std::deque<T> m_queue;  ///< Underlying container for storing the elements.
    mutable std::mutex m_mutex;  ///< Mutex to secure access to the queue.
};
