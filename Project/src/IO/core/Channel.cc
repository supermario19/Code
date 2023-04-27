#include <SCU/IO/core/Channel.h>
#include <SCU/IO/core/EventLoop.h>
#include <SCU/IO/util/Logger.h>
#include <assert.h>
#include <sys/poll.h>
#include <sstream>
using namespace SCU::IO::core;
using namespace SCU::IO::util;

const int Channel::kNoneEvent = 0;
const int Channel::kReadEvent = POLLIN | POLLPRI;
const int Channel::kWriteEvent = POLLOUT;

Channel::Channel(EventLoop* loop, int fd)
    : loop_(loop),
      fd_(fd),
      event_(kNoneEvent),
      revent_(kNoneEvent),
      index_(-1),
      tied_(false),
      logHub_(false),
      eventHandling_(false),
      addedToLoop_(false)
{
}

Channel::~Channel()
{
    assert(!eventHandling_);
    assert(!addedToLoop_);
}

void Channel::setWriteCallback(EventCallback writeCallback)
{
    writeCallback_ = writeCallback;
}
void Channel::setErrorCallback(EventCallback errorCallback)
{
    errorCallback_ = errorCallback;
}
void Channel::setCloseCallback(EventCallback closeCallback)
{
    closeCallback_ = closeCallback;
}
void Channel::setReadCallback(ReadEventCallback readCallback)
{
    readCallback_ = readCallback;
}

void Channel::handleEvent(util::Timestamp receiveTime)
{
    std::shared_ptr<void> guard;
    if (tied_) {
        guard = tie_.lock();
        if (guard) {
            handleEventWithGuard(receiveTime);
        }
    } else {
        handleEventWithGuard(receiveTime);
    }
}

void Channel::handleEventWithGuard(Timestamp receiveTime)
{
    eventHandling_ = true;
    LOG_INFO << reventsToString();

    if ((revent_ & POLLHUP) && !(revent_ & POLLIN)) {
        if (logHub_) {
            LOG_WARN << "fd:" << fd_ << " Channel::handleEvent POLLHUB";
        }
        if (closeCallback_) {
            closeCallback_();
        }
    }

    if (revent_ & POLLNVAL) {
        LOG_WARN << "fd:" << fd_ << " Channel::handleEvent POLLHUB";
    }

    if (revent_ & (POLLERR | POLLNVAL)) {
        if (errorCallback_) {
            errorCallback_();
        }
    }

    if (revent_ & (POLLIN | POLLPRI | POLLRDHUP)) {
        if (readCallback_) {
            readCallback_(receiveTime);
        }
    }

    if (revent_ & POLLOUT) {
        if (writeCallback_) {
            writeCallback_();
        }
    }

    eventHandling_ = false;
}

void Channel::tie(const std::shared_ptr<void>& obj)
{
    tied_ = true;
    tie_ = obj;
}

int Channel::fd() const
{
    return fd_;
}
int Channel::event() const
{
    return event_;
}
int Channel::index() const
{
    return index_;
}
EventLoop* Channel::ownerLoop()
{
    return loop_;
}

bool Channel::isWriting() const
{
    return event_ & kWriteEvent;
}
bool Channel::isReading() const
{
    return event_ & kReadEvent;
}
bool Channel::isNonEvent() const
{
    return event_ == kNoneEvent;
}

void Channel::enableReading()
{
    event_ |= kReadEvent;
    update();
}

void Channel::disableReading()
{
    event_ &= ~kReadEvent;
    update();
}
void Channel::enableWriting()
{
    event_ |= kWriteEvent;
    update();
}
void Channel::disableWriting()
{
    event_ &= ~kWriteEvent;
    update();
}
void Channel::disableAll()
{
    event_ = kNoneEvent;
    update();
}

void Channel::setIndex(int index)
{
    index_ = index;
}
void Channel::setRevent(int revent)
{
    revent_ = revent;
}
void Channel::doNotLogHub()
{
    logHub_ = false;
}

void Channel::remove()
{
    assert(isNonEvent());
    addedToLoop_ = false;
    loop_->removeChannel(this);
}
void Channel::update()
{
    addedToLoop_ = true;
    loop_->updateChannel(this);
}

std::string Channel::reventsToString() const
{
    return eventToString(fd_, revent_);
}

std::string Channel::eventsToString() const
{
    return eventToString(fd_, event_);
}

std::string Channel::eventToString(int fd, int ev)
{
    std::ostringstream oss;
    oss << fd << ": ";
    if (ev & POLLIN)
        oss << "IN ";
    if (ev & POLLPRI)
        oss << "PRI ";
    if (ev & POLLOUT)
        oss << "OUT ";
    if (ev & POLLHUP)
        oss << "HUP ";
    if (ev & POLLRDHUP)
        oss << "RDHUP ";
    if (ev & POLLERR)
        oss << "ERR ";
    if (ev & POLLNVAL)
        oss << "NVAL ";
    return oss.str();
}