#include <SCU/IO/core/EventLoop.h>
#include <SCU/IO/core/Poller.h>
using namespace SCU::IO::core;

Poller::Poller(EventLoop* loop) : loop_(loop) {}

Poller::~Poller() = default;

bool Poller::hasChannel(Channel* channel) const
{
    assertInLoopThread();
    auto it = channels_.find(channel->fd());
    return it != channels_.end() && it->second == channel;
}

void Poller::assertInLoopThread() const
{
    loop_->assertInLoopThread();
}
