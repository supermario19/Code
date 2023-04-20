#include <SCU/IO/util/Logger.h>
#include <SCU/IO/util/Thread.h>
#include <SCU/IO/util/CurrentThread.h>

using namespace SCU::IO::util;

void func()
{
    while (1)
    {
        LOG_INFO << "in thread";
        sleep(1);
    }
}

int main()
{
    Thread thread(func, "thread");
    LOG_INFO << CurrentThread::tid();
    thread.start();
    thread.join();
    return 0;
}