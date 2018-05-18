#ifndef __MSG_THREAD_POOL_H__
#define __MSG_THREAD_POOL_H__

namespace Pub {

enum class TP_STATUS {
    TP_READY = 0,
}

template <typename T>
class MsgThreadPool {
private:
    int max_size;
    int cur_size;
    TP_STATUS status;

    ThreadSafeQueue<T> msg_q;

    typedef struct thread_info {
        int thread_id;
        TP_STATUS status;
        int load;
    }ThreadInfo;
    ThreadSafeList<ThreadInfo> thread_list;

    void pool_monitor_handler();
public:
    explicit ThreadPool(void(thread_handler)(void), void(msg_distribute)(void), int size);
    ThreadPool(const ThreadPool&) = delete;
    ThreadPool() = delete;
    ~ThreadPool();

    void run();
    void killall();
    void status();
};

}


#endif
