#pragma once
#include <mutex>
#include <deque>
#include <iostream>
#include <optional>


template <typename T>
class tsqueue {
    public:
        tsqueue() = default;
        tsqueue(const tsqueue&) = delete;
        tsqueue& operator=(const tsqueue&) = delete;

        uint32_t size() {
            std::lock_guard<std::mutex> lock(m_mutex);
            return m_queue.size();
        }

        bool empty() {
            std::lock_guard<std::mutex> lock(m_mutex);
            return m_queue.empty();
        }

        void clear() {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_queue.clear();
        }

        std::optional<T> pop() {
            std::lock_guard<std::mutex> lock(m_mutex);
            if (m_queue.empty())
                return std::nullopt;
            T item = m_queue.front();
            m_queue.pop_front();
            return std::make_optional(item);
        }

        void push(const T& item) {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_queue.push_back(item);
        }

        ~tsqueue() {
            clear();
        }

    private:
        std::deque<T> m_queue;
        mutable std::mutex m_mutex;
};
