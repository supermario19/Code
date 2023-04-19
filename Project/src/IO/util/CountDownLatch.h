#ifndef SCU_IO_UTIL_CountDownLatch_H
#define SCU_IO_UTIL_CountDownLatch_H
#include "Condition.h"
#include "Mutex.h"
namespace SCU {
namespace IO {
namespace util {
class CountDownLatch {
public:
    CountDownLatch(int count);

    ~CountDownLatch() = default;

    void wait();

    void countDown();

private:
    int       count_;
    Mutex     mutex_;
    Condition condition_;
};
}  // namespace util
}  // namespace IO
}  // namespace SCU

#endif  // SCU_IO_UTIL_CountDownLatch_H