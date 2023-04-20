#include <SCU/IO/util/Timestamp.h>
#include <assert.h>
#include <chrono>
#include <iostream>
using namespace SCU::IO::util;
using namespace std;
using namespace std::chrono;

int main()
{
    std::cout << Timestamp::now().toString() << std::endl;
    std::cout << Timestamp::now().toFormatString() << std::endl;
    Timestamp start = Timestamp::now();
    std::cout << start.toFormatString() << std::endl;
    sleep(1);
    Timestamp end = Timestamp::now();
    std::cout << end.toFormatString() << std::endl;
    assert(start < end);
    return 0;
}