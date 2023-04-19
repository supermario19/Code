#include <SCU/IO/util/Condition.h>
#include <SCU/IO/util/Thread.h>
#include <sys/unistd.h>
#include <iostream>
using namespace SCU::IO::util;

void func(Condition &cond)
{
    sleep(10);
    cond.notify();
}

int main()
{
    Mutex mutex;
    Condition cond(mutex);
    Thread th(std::bind(func, std::ref(cond)), "thread1");
    th.start();
    cond.wait();
    std::cout << "wait() end" << std::endl;
    th.join();
    return 0;
}