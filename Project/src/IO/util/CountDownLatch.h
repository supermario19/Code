#ifndef SCU_IO_UTIL_CountDownLatch_H
#define SCU_IO_UTIL_CountDownLatch_H
#include <SCU/IO/util/Condition.h>
#include <SCU/IO/util/Mutex.h>
#include <SCU/IO/util/UnCopyable.h>
namespace SCU {
namespace IO {
namespace util {
class CountDownLatch : UnCopyable {
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