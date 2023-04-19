#include <iostream>
#include <SCU/IO/util/CurrentThread.h>

using namespace SCU::IO::util;

int main()
{
    std::cout <<  CurrentThread::tid() << std::endl;
    return 0;
}