#ifndef _GO_RUNTIME_SYNC_H
#define _GO_RUNTIME_SYNC_H

#include <mutex>
#include <condition_variable>

namespace sync {
    class Mutex : private std::mutex {
        friend class Cond;
    public:
        void Lock() {
            std::mutex::lock();
        }

        void Unlock() {
            std::mutex::unlock();
        }
    };

    class Cond {
    private:
        std::condition_variable_any cv;
    public:
        Mutex *L;
        Cond(Mutex &m) : L(&m) {}

        void Wait() {
            cv.wait(static_cast<std::mutex&>(*L));
        }

        void Signal() {
            cv.notify_one();
        }

        void Broadcast() {
            cv.notify_all();
        }
    };

    class WaitGroup {
    private:
        std::mutex m;
        std::condition_variable cv;
        int waiters;
    public:
        void Add(int delta) {
            std::unique_lock<std::mutex> lk(m);
            waiters += delta;
            if (waiters <= 0) {
                // this should panic if waiters < 0
                // but let's start with this
                cv.notify_all();
            }
        }

        void Done() {
            Add(-1);
        }

        void Wait() {
            std::unique_lock<std::mutex> lk(m);
            while (waiters > 0) cv.wait(lk);
        }
    };
}

#endif
