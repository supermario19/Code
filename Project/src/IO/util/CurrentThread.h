#ifndef SCU_IO_UTIL_CURRENTTHREAD_H
#define SCU_IO_UTIL_CURRENTTHREAD_H

namespace SCU {
namespace IO {
namespace util {

namespace CurrentThread {
extern __thread int t_cacheTid;

extern void cacheTid();

inline int tid()
{
    if (t_cacheTid == 0) {
        cacheTid();
    }
    return t_cacheTid;
}

}  // namespace CurrentThread

}  // namespace util
}  // namespace IO
}  // namespace SCU

#endif  // SCU_IO_UTIL_CURRENTTHREAD_H
