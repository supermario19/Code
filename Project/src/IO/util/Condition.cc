#include <SCU/IO/util/Condition.h>

using namespace SCU::IO::util;

Condition::Condition(Mutex& mutex) : mutex_(mutex) {}

Condition::~Condition() {}

void Condition::notify()
{
    condition_.notify_one();
}

void Condition::notifyAll()
{
    condition_.notify_all();
}

void Condition::wait()
{
    condition_.wait(mutex_);
}