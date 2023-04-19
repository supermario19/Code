#include "Condition.h"

using namespace SCU::IO::util;

Condition::Condition(Mutex &mutex) : mutex_(mutex) {}

Condition::~Condition() {}

void Condition::notify()
{
    condition_.notify_one();
}

void Condition::notifyAll()
{
    condition_.notify_all();
}

void Condition::wait() {
    std::unique_lock<std::mutex> lock(mutex_.getMutex());
    condition_.wait(lock);
}