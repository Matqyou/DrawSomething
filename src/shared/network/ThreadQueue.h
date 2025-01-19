//
// Created by Matq on 19/01/2025.
//

#pragma once

#include <asio.hpp>
#include <mutex>
#include <deque>

template<typename T>
class ThreadQueue {
protected:
    std::mutex muxQueue;
    std::deque<T> deqQueue;
    std::condition_variable cvBlocking;
    std::mutex muxBlocking;

public:
    ThreadQueue() = default;
    ThreadQueue(const ThreadQueue<T>&) = delete;
    virtual ~ThreadQueue() { clear(); }

public:
    const T& front() {
        std::scoped_lock lock(muxQueue);
        return deqQueue.front();
    }
    const T& back() {
        std::scoped_lock lock(muxQueue);
        return deqQueue.back();
    }
    T pop_front() {
        std::scoped_lock lock(muxQueue);
        auto t = std::move(deqQueue.front());
        deqQueue.pop_front();
        return t;
    }
    T pop_back() {
        std::scoped_lock lock(muxQueue);
        auto t = std::move(deqQueue.back());
        deqQueue.pop_back();
        return t;
    }
    void push_back(const T& item) {
        std::scoped_lock lock(muxQueue);
        deqQueue.emplace_back(std::move(item));

        std::unique_lock<std::mutex> ul(muxBlocking);
        cvBlocking.notify_one();
    }
    void push_front(const T& item) {
        std::scoped_lock lock(muxQueue);
        deqQueue.emplace_front(std::move(item));

        std::unique_lock<std::mutex> ul(muxBlocking);
        cvBlocking.notify_one();
    }
    bool empty() {
        std::scoped_lock lock(muxQueue);
        return deqQueue.empty();
    }
    size_t count() {
        std::scoped_lock lock(muxQueue);
        return deqQueue.size();
    }
    void clear() {
        std::scoped_lock lock(muxQueue);
        deqQueue.clear();
    }
    void wait() {
        while (empty()) {
            std::unique_lock<std::mutex> ul(muxBlocking);
            cvBlocking.wait(ul);
        }
    }

};
