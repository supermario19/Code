#ifndef SCU_IO_UTIL_CONDITION_H
#define SCU_IO_UTIL_CONDITION_H
#include <SCU/IO/util/Mutex.h>
#include <SCU/IO/util/UnCopyable.h>
#include <condition_variable>

namespace SCU {
namespace IO {
namespace util {
class Condition : UnCopyable {
public:
    Condition(Mutex& mutex);
    ~Condition();

    void notify();

    void notifyAll();

    void wait();

private:
    Mutex&                  mutex_;
    std::condition_variable condition_;
};
}  // namespace util
}  // namespace IO
}  // namespace SCU

#endif  // SCU_IO_UTIL_CONDITION_H