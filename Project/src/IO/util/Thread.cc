#include <SCU/IO/util/Thread.h>
#include <SCU/IO/util/CurrentThread.h>

using namespace SCU::IO::util;

Thread::Thread(ThreadFunc func, const std::string &name)
 : name_(name), func_(func), th_(), started_(false), tid_(0), latch_(1)
{

}

Thread::~Thread()
{
    if (started_ && !th_.joinable())
    {
        th_.detach();
    }
}

void Thread::start()
{
    started_ = true;
    th_ = std::thread(&Thread::runInThread, this);
    latch_.wait();
}

void Thread::join()
{
    if (th_.joinable())
    {
        th_.join();
    } else {
        th_.detach();
    }
}

void Thread::runInThread(Thread *th)
{
    th->tid_ = CurrentThread::tid();
    th->latch_.countDown();
    try {
        if (th->func_)
        {
            th->func_();
            th->started_ = false;
        }
    } catch (std::exception &e)
    {
        th->started_ = false;
        std::cerr << e.what() << std::endl;
    }
}

