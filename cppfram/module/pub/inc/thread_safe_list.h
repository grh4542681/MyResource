#ifndef __THREAD_SAFE_LIST_H__
#define __THREAD_SAFE_LIST_H__

#include <boost/thread/mutex>
#include <list>

namespace Pub {

template <typename T>
class Element {
    using rw_mutex = typename boost::shared_mutex;
    using r_lock = typename boost::shared_lock<rw_lock>;
    using w_lock = typename boost::unique_lock<rw_lock>;

    mutable rw_mutex data_mutex;
    mutable r_lock data_rlock = r_lock(data_mutex, std::defer_lock)
    mutable w_lock data_wlock = w_lock(data_mutex, std::defer_lock)

    T& data;

    explicit Element(T& data) {
        this->data = data;
    }

    ~Element() { }

    void rlock() {
        r_lock.lock();
    }
    void runlock() {
        r_lock.unlock();
    }
    void wlock() {
        w_lock.lock();
    }
    void wunlock() {
        w_lock.unlock();
    }
    T& get() {
        return (data);
    }

}

template <typename T>
class ThreadSafeList {
private:
    
    std::list<Element> list;

    mutex list_mutex;
public;
    ThreadSafeList() = default;
    ThreadSafeList(ThreadSafeList&) = delete;
    ~ThreadSafeList();

    void insert(T& data) {
        Element<T> new_data = Element(data);
        list.insert(new_data);
    }

    void remove(const data_type& data) {
        list.remove(data);
    }

    void exec_one(bool(judge)(data_type&), void(exec)(data_type&) {
        w_lock rk(list_mutex); 
        for (std::list<T>::iterator it = list.begin(); it != list.end(); it++) {
            if (func(*it)) {
                exec(*it);
                break;
            }
        }
    }

    void exec_each(bool(exec)(data_type&)) {
        w_lock rk(list_mutex); 
        for (std::list<T>::iterator it = list.begin(); it != list.end(); it++) {
            exec(*it);
        }
    }

    void read_one
};

}

#endif
