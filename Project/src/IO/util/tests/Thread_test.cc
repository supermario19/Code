#include <SCU/IO/util/Thread.h>
#include <SCU/IO/util/CurrentThread.h>
#include <iostream>

void func1()
{
    std::cout << "func1()" << std::endl;
}

void func2(int a, int b, int c)
{
    std::cout << a + b + c << std::endl;
}

void func3(SCU::IO::util::Thread *th)
{
    std::cout << "tid = " << th->tid() << std::endl;
}

int main()
{
    // 测试1
    {
        SCU::IO::util::Thread thread(func1, "func1 thread()");
        thread.start();
        thread.join();
    }

    // 测试2
    {
        SCU::IO::util::Thread thread(std::bind(func2, 1, 2, 3), "func2 thread()");
        std::cout << "thread.started()" << thread.started() << std::endl;
        thread.start();
        std::cout << "thread.started()" << thread.started() << std::endl;
        thread.join();
    }

    // 测试3
    {
        SCU::IO::util::Thread thread(std::bind(func3, &thread), "func3 thread()");
        thread.start();
        thread.join();
        std::cout << "main tid = " << SCU::IO::util::CurrentThread::tid() << std::endl;
    }

    return 0;
}