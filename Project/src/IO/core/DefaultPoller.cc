#include <SCU/IO/core/EpollPoller.h>
#include <SCU/IO/core/Poller.h>

using namespace SCU::IO::core;

Poller* Poller::newDefaultPoller(EventLoop* loop)
{
    return new EpollPoller(loop);
}