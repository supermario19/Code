#ifndef SCU_IO_UTIL_CONDITION_H
#define SCU_IO_UTIL_CONDITION_H
#include "Mutex.h"
#include <condition_variable>

namespace SCU {
namespace IO {
namespace util {
class Condition {
public:
    Condition(Mutex &mutex);
    ~Condition();

    void notify();

    void notifyAll();

    void wait();
private:
    Mutex &mutex_;
    std::condition_variable condition_;
};
}  // namespace util
}  // namespace IO
}  // namespace SCU

#endif  // SCU_IO_UTIL_CONDITION_H