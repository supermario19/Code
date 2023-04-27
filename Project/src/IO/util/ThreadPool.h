#ifndef SCU_IO_UTIL_THREADPOOL_H
#define SCU_IO_UTIL_THREADPOOL_H
#include <SCU/IO/util/Condition.h>
#include <SCU/IO/util/UnCopyable.h>
#include <deque>
#include <functional>
#include <memory>
#include <vector>
namespace SCU {
namespace IO {
namespace util {
class Thread;
class ThreadPool : UnCopyable {
    using Task = std::function<void()>;

public:
    explicit ThreadPool(
        const std::string& nameArg = std::string("ThreadPool"));
    ~ThreadPool();

    void setThreadInitCallback(const Task& cb)
    {
        threadInitCallback_ = cb;
    }

    void start(int numThreads);

    void stop();

    const std::string& name() const
    {
        return name_;
    }

    void run(Task task);

private:
    void runInThread();
    Task take();

private:
    mutable Mutex                        mutex_;
    Condition                            notEmpty_;
    Condition                            notFull_;
    std::string                          name_;
    bool                                 running_;
    std::vector<std::unique_ptr<Thread>> threads_;
    std::deque<Task>                     queue_;
    Task                                 threadInitCallback_;
};
}  // namespace util
}  // namespace IO
}  // namespace SCU

#endif  // SCU_IO_UTIL_THREADPOOL_H