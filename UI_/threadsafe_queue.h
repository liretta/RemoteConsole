#pragma once
#include <queue>
#include <mutex>

template <typename T>
class ThreadSafeQueue
{
public:

    void push(const T& elem)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_queue.push(elem);
    }

    T pop()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto value = m_queue.front();
        m_queue.pop();
        return value;
    }

    bool empty() const
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_queue.empty();
    }

private:

    mutable std::mutex  m_mutex;
    std::queue<T>       m_queue;
};
