#include <SCU/IO/util/Logger.h>
#include <SCU/IO/util/Thread.h>
#include <SCU/IO/util/ThreadPool.h>
#include <unistd.h>
#include <iostream>
using namespace SCU::IO::util;

void func(int a, int b)
{
    std::cout << "a + b = " << a + b << std::endl;
}

void addTask(ThreadPool& pool)
{
    for (int i = 0; i < 100; i++) {
        pool.run(std::bind(func, 1, 2));
    }
}

int main()
{
    ThreadPool threadPool;
    Thread     th(std::bind(addTask, std::ref(threadPool)), " ");
    threadPool.start(5);
    th.start();
    LOG_INFO << "start end";

    while (1) {
    }

    return 0;
}