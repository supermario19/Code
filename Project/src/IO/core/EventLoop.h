#ifndef SCU_IO_CORE_EVENTLOOP_H
#define SCU_IO_CORE_EVENTLOOP_H
#include <SCU/IO/core/Channel.h>
#include <SCU/IO/core/Poller.h>
#include <SCU/IO/util/Mutex.h>
#include <SCU/IO/util/Timestamp.h>
#include <SCU/IO/util/UnCopyable.h>
#include <memory>
namespace SCU {
namespace IO {
namespace core {
class EventLoop : UnCopyable {
    using Functor = std::function<void()>;
    using ChannelList = std::vector<Channel*>;

public:
    EventLoop();
    ~EventLoop();

    void loop();

    void quit();

    void runInLoop(Functor cb);
    void queueInLoop(Functor cb);

    void wakeUp();

    void              updateChannel(Channel* channel);
    void              removeChannel(Channel* channel);
    bool              hasChannel(Channel* channel);
    bool              isInLoopThread() const;
    bool              eventHandling() const;
    static EventLoop* getEventLoopOfCurrentThread();
    util::Timestamp   pollRetureTime() const;
    void              assertInLoopThread();

private:
    void handleRead();

    void doPendingFunctors();

    void abortNotInLoopThread();

    void printActiveChannels() const;

private:
    bool                     looping_;
    bool                     quit_;
    bool                     eventHandling_;
    bool                     callingPendingFunctor_;
    const int                tid_;
    util::Timestamp          pollReturnTime_;
    std::unique_ptr<Poller>  poller_;
    int                      wakeupFd_;
    std::unique_ptr<Channel> wakeupChannels_;
    ChannelList              activeChannels_;
    Channel*                 currentActiveChannel_;
    mutable util::Mutex      mutex_;
    std::vector<Functor>     pendingFunctors_;
};
}  // namespace core
}  // namespace IO
}  // namespace SCU

#endif  // SCU_IO_CORE_EVENTLOOP_H