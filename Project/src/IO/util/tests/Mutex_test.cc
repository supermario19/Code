#include <SCU/IO/util/CurrentThread.h>
#include <SCU/IO/util/Mutex.h>
#include <SCU/IO/util/Thread.h>
#include <iostream>
#include <vector>

using namespace SCU::IO::util;

int g_number = 0;

void add_number(int* num, Mutex* mutex)
{
    for (int i = 0; i < 1000; i++) {
        {
            MutexGuard lock(*mutex);
            (*num)++;
            std::cout << "num = " << *num
                      << " tid = " << CurrentThread::tid() << std::endl;
        }
    }
}

int main()
{
    Mutex  mutex;
    std::vector<Thread *> threads;
    for (int i = 0; i < 10; i++) {
        Thread *thi = new Thread(std::bind(add_number, &g_number, &mutex),
                  std::string("thread" + i));
        threads.push_back(thi);
    }

    for (int i = 0; i < 10; i++)
    {
        threads[i]->start();
    }

    for (int i = 0; i < 10; i++)
    {
        threads[i]->join();
        delete threads[i];
    }

    return 0;
}
