#ifndef SCU_IO_UTIL_CURRENTTHREAD_H
#define SCU_IO_UTIL_CURRENTTHREAD_H

namespace SCU {
namespace IO {
namespace util {

namespace CurrentThread {

extern __thread int         t_cacheTid;
extern __thread char        t_tidString[32];
extern __thread int         t_tidStringLength;
extern __thread const char* t_threadName;
extern __thread char        t_errnoBuf[512];
extern __thread char        t_time[64];

extern void cacheTid();

inline int tid()
{
    if (t_cacheTid == 0) {
        cacheTid();
    }
    return t_cacheTid;
}

inline char* tidString()
{
    return t_tidString;
}

inline int tidStringLength()
{
    return t_tidStringLength;
}

inline const char* threadName()
{
    return t_threadName;
}

}  // namespace CurrentThread

}  // namespace util
}  // namespace IO
}  // namespace SCU

#endif  // SCU_IO_UTIL_CURRENTTHREAD_H
