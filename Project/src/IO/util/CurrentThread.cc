#include <SCU/IO/util/CurrentThread.h>
#include <string.h>
#include <syscall.h>
#include <unistd.h>
#include <string>
namespace SCU {
namespace IO {
namespace util {
namespace CurrentThread {

__thread int         t_cacheTid = 0;
__thread char        t_tidString[32];
__thread int         t_tidStringLength = 6;
__thread const char* t_threadName = "unknow";

void cacheTid()
{
    if (t_cacheTid == 0) {
        t_cacheTid = static_cast<int>(::syscall(SYS_gettid));
        t_tidStringLength = snprintf(t_tidString, 6, "%5d", t_cacheTid);
    }
}

}  // namespace CurrentThread
}  // namespace util
}  // namespace IO
}  // namespace SCU