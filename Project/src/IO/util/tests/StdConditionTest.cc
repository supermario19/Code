#include <condition_variable>
#include <mutex>

using namespace std;

int main()
{
    std::mutex                  muetx_;
    std::condition_variable_any conditional_;
    conditional_.wait(muetx_);
    return 0;
}