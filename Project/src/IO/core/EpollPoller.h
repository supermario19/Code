#ifndef SCU_IO_CORE_EPOLLPOLLER_H
#define SCU_IO_CORE_EPOLLPOLLER_H
#include <SCU/IO/core/Poller.h>
#include <SCU/IO/util/Timestamp.h>
#include <sys/epoll.h>
namespace SCU {
namespace IO {
namespace core {
class EpollPoller : public Poller {
    using EventList = std::vector<struct epoll_event>;

public:
    EpollPoller(EventLoop* loop);
    ~EpollPoller();

    virtual util::Timestamp poll(int          timeoutMs,
                                 ChannelList* activeChannels) override;

    virtual void updateChannel(Channel* channel) override;
    virtual void removeChannel(Channel* channel) override;

private:
    static const int   kInitEventListSize = 16;
    static const char* operationToString(int operation);
    void               fillActiveChannels(int          numEvents,
                                          ChannelList* activeChannels) const;

    void update(int operation, Channel* channel);

private:
    int       epollfd_;
    EventList events;
};
}  // namespace core
}  // namespace IO
}  // namespace SCU

#endif  // SCU_IO_CORE_EPOLLPOLLER_H