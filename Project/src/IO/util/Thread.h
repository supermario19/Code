#ifndef SCU_IO_UTIL_THREAD_H
#define SCU_IO_UTIL_THREAD_H
#include <SCU/IO/util/CountDownLatch.h>
#include <SCU/IO/util/UnCopyable.h>
#include <functional>
#include <iostream>
#include <string>
#include <thread>

namespace SCU {
namespace IO {
namespace util {
class Thread : UnCopyable {
    using ThreadFunc = std::function<void()>;

public:
    Thread() = default;

    Thread(ThreadFunc func, const std::string& name);

    ~Thread();

    void start();

    void join();

    const std::string& name() const
    {
        return name_;
    }

    bool started() const
    {
        return started_;
    }

    int tid() const
    {
        return tid_;
    }

private:
    static void runInThread(Thread* th);

private:
    std::string name_;
    ThreadFunc  func_;
    std::thread th_;
    bool        started_;
    int         tid_;

private:
    CountDownLatch latch_;
};
}  // namespace util
}  // namespace IO
}  // namespace SCU

#endif  // SCU_IO_UTIL_THREAD_H