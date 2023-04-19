#ifndef SCU_IO_UTIL_MUTEX_H
#define SCU_IO_UTIL_MUTEX_H
#include <SCU/IO/util/UnCopyable.h>
#include <assert.h>
#include <mutex>
namespace SCU {
namespace IO {
namespace util {
class Mutex : UnCopyable {
public:
    Mutex() : holder_(0) {}

    ~Mutex()
    {
        assert(holder_ == 0);
    }

    std::mutex& getMutex()
    {
        return mutex_;
    }

    void lock();

    void unlock();

    bool isLockedByThisThread() const;

    void assertLock() const;

private:
    pid_t      holder_;
    std::mutex mutex_;
};

class MutexGuard : UnCopyable {
public:
    explicit MutexGuard(Mutex& mutex) : mutex_(mutex)
    {
        mutex_.lock();
    }

    ~MutexGuard()
    {
        mutex_.unlock();
    }

private:
    Mutex& mutex_;
};

}  // namespace util
}  // namespace IO
}  // namespace SCU

#endif  // SCU_IO_UTIL_MUTEX_H