#include <SCU/IO/util/CountDownLatch.h>

using namespace SCU::IO::util;

CountDownLatch::CountDownLatch(int count)
    : count_(count), mutex_(), condition_(mutex_)
{
}

void CountDownLatch::wait()
{
    condition_.wait();
}

void CountDownLatch::countDown()
{
    MutexGuard lock(mutex_);   
    if (--count_ == 0)
    {
        condition_.notify();
    }
}
