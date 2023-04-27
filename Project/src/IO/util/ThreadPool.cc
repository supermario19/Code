#include <SCU/IO/util/CurrentThread.h>
#include <SCU/IO/util/Logger.h>
#include <SCU/IO/util/Thread.h>
#include <SCU/IO/util/ThreadPool.h>
using namespace SCU::IO::util;

ThreadPool::ThreadPool(const std::string& nameArg)
    : mutex_(),
      notEmpty_(mutex_),
      notFull_(mutex_),
      name_(nameArg),
      running_(false)
{
}

ThreadPool::~ThreadPool()
{
    if (running_) {
        stop();
    }
}

void ThreadPool::start(int numThreads)
{
    assert(threads_.empty());
    running_ = true;
    threads_.reserve(numThreads);
    for (int i = 0; i < numThreads; i++) {
        char buff[32];
        snprintf(buff, sizeof buff, "%d", i + 1);
        threads_.emplace_back(new Thread(
            std::bind(&ThreadPool::runInThread, this), name_ + buff));
        threads_[i]->start();
    }
    if (numThreads == 0 && threadInitCallback_) {
        threadInitCallback_();
    }

    LOG_INFO << "ThreadPool" + name_ + " is start";
}

void ThreadPool::stop()
{
    {
        MutexGuard lock(mutex_);
        running_ = false;
        notEmpty_.notifyAll();
        notFull_.notifyAll();
    }

    for (auto& th : threads_) {
        th->join();
    }
    LOG_INFO << "ThreadPool" + name_ + " is end";
}

void ThreadPool::run(Task task)
{
    if (threads_.empty()) {
        task();
    } else {
        {
            MutexGuard lock(mutex_);
            if (!running_)
                return;
            queue_.push_back(std::move(task));
        }
        notEmpty_.notify();
    }
    LOG_INFO << "Thread add task";
}

void ThreadPool::runInThread()
{
    try {
        if (threadInitCallback_) {
            threadInitCallback_();
        }
        while (running_) {
            LOG_INFO << "Thread take task";
            Task task(take());
            if (task) {
                task();
            }
        }
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        abort();
    }
}

ThreadPool::Task ThreadPool::take()
{
    MutexGuard lock(mutex_);
    while (queue_.empty() && running_) {
        notEmpty_.wait();
    }
    Task task;
    if (!queue_.empty()) {
        task = queue_.front();
        queue_.pop_front();
    }
    return task;
}
