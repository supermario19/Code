#ifndef SCU_IO_CORE_POLLER_H
#define SCU_IO_CORE_POLLER_H
#include <SCU/IO/util/Timestamp.h>
#include <SCU/IO/util/UnCopyable.h>
#include <unordered_map>
#include <vector>
namespace SCU {
namespace IO {
namespace core {
class EventLoop;
class Channel;
class Poller : UnCopyable {
public:
    using ChannelList = std::vector<Channel*>;

    Poller(EventLoop* loop);
    virtual ~Poller();

    virtual util::Timestamp poll(int          timeOutMs,
                                 ChannelList* activeChannels) = 0;

    virtual void updateChannel(Channel* channel) = 0;
    virtual void removeChannel(Channel* channel) = 0;

    virtual bool hasChannel(Channel* channel) const;

    static Poller* newDefaultPoller(EventLoop* loop);

    void assertInLoopThread() const;

protected:
    using ChannelMap = std::unordered_map<int, Channel*>;

    ChannelMap channels_;

private:
    EventLoop* loop_;
};
}  // namespace core
}  // namespace IO
}  // namespace SCU

#endif  // SCU_IO_CORE_POLLER_H