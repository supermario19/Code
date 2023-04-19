#include <SCU/IO/util/CountDownLatch.h>
#include <SCU/IO/util/Thread.h>
#include <iostream>
using namespace SCU::IO::util;

void func1(CountDownLatch& latch)
{
    latch.countDown();
    std::cout << "thread1 end" << std::endl;
}

void func2(CountDownLatch& latch)
{
    latch.wait();
    std::cout << "thread2 end" << std::endl;
}

void func3(CountDownLatch& latch)
{
    std::cout << "thread3 end" << std::endl;
}

void func4(CountDownLatch& latch)
{
    std::cout << "thread4 end" << std::endl;
}

int main()
{
    {
        CountDownLatch latch(1);
        Thread thread1(std::bind(func1, std::ref(latch)), "thread1");
        Thread thread2(std::bind(func2, std::ref(latch)), "thread2");
        thread1.start();
        thread2.start();
        thread1.join();
        thread2.join();
    }
    {
        CountDownLatch latch(1);
        Thread thread1(std::bind(func3, std::ref(latch)), "thread1");
        Thread thread2(std::bind(func4, std::ref(latch)), "thread2");
        thread1.start();
        thread2.start();
        thread1.join();
        thread2.join();
    }
    return 0;
}