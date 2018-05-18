#ifndef __THREAD_SAFE_QUEUE__
#define __THREAD_SAFE_QUEUE__

#include <queue>
#include <mutex>
#include <condition_variable>

#include <pub_exception.h>

namespace Pub {

template<typename T>
class ThreadSafeQueue{
private:
    mutable std::mutex q_mutex;
    mutable std::condition_variable q_cond;
    std::queue<T> data_queue;
public:
    using value_type = typename std::queue<T>::value_type;
    using container_type = typename std::queue<T>::container_type;
    ThreadSafeQueue() = default;
    ThreadSafeQueue(const ThreadSafeQueue&) = delete;
    ThreadSafeQueue& operator=(const ThreadSafeQueue&) = delete;
    explicit ThreadSafeQueue(T first, T last) {
        for (auto it = first;it != last;it++) {
            this->data_queue.push(*it);
        }
    }

    explicit ThreadSafeQueue(const typename std::queue<T>::container_type &c):data_queue(c) { }

    ~ThreadSafeQueue(){ } 

    T& pop() {
        std::lock_guard<std::mutex> lk(q_mutex);
        auto value = std::move(data_queue.front());
        data_queue.pop();
        return (value);
    }

    T& pop_wait() {
        std::unique_lock<std::mutex> uk(q_mutex);
        q_cond.wait(uk, !(this->data_queue.empty()));
        auto value = std::move(data_queue.front());
        data_queue.pop();
        return (value);
    }

    void push(T &value) {
        std::lock_guard<std::mutex> lk(q_mutex);
        data_queue.push(std::move(value));
        q_cond.notify_one();
    }

    auto empty() const->decltype(data_queue.empty()) {
        std::lock_guard<std::mutex> lk(q_mutex);
        return (this->data_queue.empty());
    }

    auto size() const->decltype(data_queue.size()) {
        std::lock_guard<std::mutex> lk(q_mutex);
        return (this->data_queue.size());
    }
};

}



#endif
