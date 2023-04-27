#ifndef SCU_IO_CORE_CHANNEL_H
#define SCU_IO_CORE_CHANNEL_H
#include <SCU/IO/util/Timestamp.h>
#include <SCU/IO/util/UnCopyable.h>
#include <functional>
#include <memory>
namespace SCU {
namespace IO {
namespace core {
class EventLoop;
class Channel : UnCopyable {
    using EventCallback = std::function<void()>;
    using ReadEventCallback = std::function<void(util::Timestamp)>;

public:
    Channel(EventLoop* loop, int fd);
    ~Channel();

    void handleEvent(util::Timestamp receiveTime);

    void setWriteCallback(EventCallback writeCallback);
    void setErrorCallback(EventCallback errorCallback);
    void setCloseCallback(EventCallback closeCallback);
    void setReadCallback(ReadEventCallback readCallback);

    void tie(const std::shared_ptr<void>&);

    int        fd() const;
    int        event() const;
    int        index() const;
    EventLoop* ownerLoop();

    bool isWriting() const;
    bool isReading() const;
    bool isNonEvent() const;

    void enableReading();
    void disableReading();
    void enableWriting();
    void disableWriting();
    void disableAll();

    void setIndex(int index);
    void setRevent(int revent);
    void doNotLogHub();

    void remove();
    void update();

    std::string reventsToString() const;
    std::string eventsToString() const;

private:
    void handleEventWithGuard(util::Timestamp receiveTime);

private:
    EventLoop* loop_;
    const int  fd_;
    int        event_;
    int        revent_;
    int        index_;

    // 回调函数
    EventCallback     writeCallback_;
    EventCallback     errorCallback_;
    EventCallback     closeCallback_;
    ReadEventCallback readCallback_;

    // 生命周期相关
    bool                tied_;
    std::weak_ptr<void> tie_;

    bool logHub_;
    bool eventHandling_;
    bool addedToLoop_;

private:
    static const int   kNoneEvent;
    static const int   kReadEvent;
    static const int   kWriteEvent;
    static std::string eventToString(int fd, int event);
};
}  // namespace core
}  // namespace IO
}  // namespace SCU

#endif  // SCU_IO_CORE_CHANNEL_H