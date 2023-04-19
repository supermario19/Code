#include "CurrentThread.h"
#include <syscall.h>
#include <unistd.h>

namespace SCU {
namespace IO {
namespace util {
namespace CurrentThread {
    
__thread int t_cacheTid;

void cacheTid()
{
    if (t_cacheTid == 0) {
        t_cacheTid = static_cast<int>(::syscall(SYS_gettid));
    }
}

}  // namespace CurrentThread
}  // namespace util
}  // namespace IO
}  // namespace SCU