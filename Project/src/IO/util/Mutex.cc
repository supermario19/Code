#include "Mutex.h"
#include "CurrentThread.h"
using namespace SCU::IO::util;

void Mutex::lock()
{
    assert(holder_ == 0);
    holder_ = CurrentThread::tid();
    mutex_.lock();
}

void Mutex::unlock()
{
    holder_ = 0;
    mutex_.unlock();
}

bool Mutex::isLockedByThisThread() const
{
    return holder_ == CurrentThread::tid();
}

void Mutex::assertLock() const
{
    return assert(isLockedByThisThread());
}