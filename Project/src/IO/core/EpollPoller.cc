#include <SCU/IO/core/Channel.h>
#include <SCU/IO/core/EpollPoller.h>
#include <SCU/IO/util/Logger.h>
#include <assert.h>

using namespace SCU::IO::core;
using namespace SCU::IO::util;

const int kNew = -1;     // 不在map中,且不在tree中
const int kAdded = 1;    // 在map中，且在tree中
const int kDeleted = 2;  // 在map中，不在tree中

EpollPoller::EpollPoller(EventLoop* loop)
    : Poller(loop),
      epollfd_(::epoll_create1(EPOLL_CLOEXEC)),
      events(kInitEventListSize)
{
    if (epollfd_ < 0) {
        LOG_FATAL << "epoll_create error: " << errno;
    }
}

EpollPoller::~EpollPoller()
{
    ::close(epollfd_);
}

Timestamp EpollPoller::poll(int timeoutMs, ChannelList* activeChannels)
{
    int numEvents =
        ::epoll_wait(epollfd_, events.data(),
                     static_cast<int>(events.size()), timeoutMs);
    int       savedErrno = errno;
    Timestamp now(Timestamp::now());
    if (numEvents > 0) {
        // 填充活跃通道
        fillActiveChannels(numEvents, activeChannels);
        if (static_cast<size_t>(numEvents) == events.size()) {
            events.resize(events.size() * 2);
        }
    } else if (numEvents == 0) {
        LOG_INFO << "nothing happened";
    } else {
        if (savedErrno != EINTR) {
            errno = savedErrno;
            LOG_SYSERR << "EpollPoller::poll()";
        }
    }
    return now;
}

void EpollPoller::updateChannel(Channel* channel)
{
    Poller::assertInLoopThread();
    const int index = channel->index();
    LOG_TRACE << "fd = " << channel->fd()
              << " events = " << channel->event() << " index = " << index;
    if (index == kNew || index == kDeleted) {
        int fd = channel->fd();
        if (index == kNew) {
            assert(channels_.find(fd) == channels_.end());
            channels_[fd] = channel;
        } else {
            assert(channels_.find(fd) != channels_.end());
            assert(channels_[fd] == channel);
        }
        channel->setIndex(kAdded);
        update(EPOLL_CTL_ADD, channel);
    } else {
        int fd = channel->fd();
        (void)fd;
        assert(channels_.find(fd) != channels_.end());
        assert(channels_[fd] == channel);
        assert(index == kAdded);
        if (channel->isNonEvent()) {
            update(EPOLL_CTL_DEL, channel);
        } else {
            update(EPOLL_CTL_MOD, channel);
        }
    }
}
void EpollPoller::removeChannel(Channel* channel)
{
    Poller::assertInLoopThread();
    int fd = channel->fd();
    int index = channel->index();
    {
        LOG_TRACE << "fd = " << fd;
        assert(channels_.find(fd) != channels_.end());
        assert(channels_[fd] == channel);
        assert(channel->isNonEvent());
        assert(index == kAdded || index == kDeleted);
        size_t n = channels_.erase(fd);
        (void)n;
        assert(n == 1);
    }
    if (index == kAdded) {
        update(EPOLL_CTL_DEL, channel);
    }
    channel->setIndex(kNew);
}

const char* EpollPoller::operationToString(int operation)
{
    switch (operation) {
    case EPOLL_CTL_ADD:
        return "EPOLL_CTL_ADD";
    case EPOLL_CTL_DEL:
        return "EPOLL_CTL_DEL";
    case EPOLL_CTL_MOD:
        return "EPOLL_CTL_MOD";
    default:
        return "Unknown Operation";
    }
}
void EpollPoller::fillActiveChannels(int          numEvents,
                                     ChannelList* activeChannels) const
{
    assert(numEvents <= events.size());
    for (int i = 0; i < numEvents; i++) {
        Channel* channel = static_cast<Channel*>(events[i].data.ptr);
        {
            int  fd = channel->fd();
            auto it = channels_.find(fd);
            (void)it->first;
            assert(it != channels_.end());
            assert(it->second == channel);
        }
        channel->setRevent(events[i].events);
        activeChannels->push_back(channel);
    }
}

void EpollPoller::update(int operation, Channel* channel)
{
    struct epoll_event event;
    memset(&event, 0, sizeof(struct epoll_event));
    event.events = operation;
    event.data.ptr = channel;
    int fd = channel->fd();
    LOG_TRACE << "epoll_ctl op = " << operationToString(operation)
              << " fd = " << fd << " event = { "
              << channel->eventsToString() << " }";
    if (::epoll_ctl(epollfd_, operation, fd, &event) < 0) {
        if (operation == EPOLL_CTL_DEL) {
            LOG_SYSERR << "epoll_ctl op =" << operationToString(operation)
                       << " fd =" << fd;
        } else {
            LOG_SYSFATAL << "epoll_ctl op =" << operationToString(operation)
                         << " fd =" << fd;
        }
    }
}