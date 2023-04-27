#include <SCU/IO/core/EventLoop.h>
#include <SCU/IO/util/CurrentThread.h>
#include <SCU/IO/util/Logger.h>
#include <sys/eventfd.h>

using namespace SCU::IO::util;
using namespace SCU::IO::core;

namespace {
__thread EventLoop* t_loopInThisThread = 0;
}

int createEventFd()
{
    int evfd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    if (evfd < 0) {
        LOG_SYSERR << "Failed in eventfd";
        abort();
    }
    return evfd;
}

EventLoop::EventLoop()
    : looping_(false),
      quit_(false),
      eventHandling_(false),
      callingPendingFunctor_(false),
      tid_(CurrentThread::tid()),
      poller_(Poller::newDefaultPoller(this)),
      wakeupFd_(createEventFd()),
      wakeupChannels_(new Channel(this, wakeupFd_))
{
    LOG_INFO << "EventLoop createed " << this << " in thread(tid = " << tid_
             << ")";
    // 检测EventLoop合法性
    if (t_loopInThisThread) {
        LOG_FATAL << "Another EventLoop "
                  << static_cast<void*>(t_loopInThisThread)
                  << " exists in this thread(tid = " << tid_ << ")";
    } else {
        t_loopInThisThread = this;
    }
    // 创建唤醒事件
    wakeupChannels_->setReadCallback(
        std::bind(&EventLoop::handleRead, this));
    wakeupChannels_->enableReading();
}

EventLoop::~EventLoop()
{
    LOG_INFO << "EventLoop destory " << this << " in thread(tid = " << tid_
             << ")";
    wakeupChannels_->disableAll();
    wakeupChannels_->remove();
    ::close(wakeupFd_);
    t_loopInThisThread = NULL;
}

void EventLoop::loop()
{
    // 合法性检查
    {
        assert(!looping_);
        assertInLoopThread();
    }

    looping_ = true;
    quit_ = false;
    LOG_INFO << "EventLoop " << this << " in thread(tid = " << tid_ << ")"
             << " is start";
    while (!quit_) {
        // 1. 获取活跃事件
        activeChannels_.clear();
        pollReturnTime_ = poller_->poll(1000, &activeChannels_);
        if (Logger::logLevel() <= Logger::INFO) {
            // 打印活跃事件
            printActiveChannels();
        }

        //  2. 处理活跃事件
        eventHandling_ = true;
        for (Channel* channel : activeChannels_) {
            currentActiveChannel_ = channel;
            currentActiveChannel_->handleEvent(pollReturnTime_);
        }
        currentActiveChannel_ = nullptr;
        eventHandling_ = false;

        // 3. 处理上层回调
        doPendingFunctors();
    }

    LOG_INFO << "EventLoop " << this << " in thread(tid = " << tid_ << ")"
             << " is end";
    looping_ = false;
}

void EventLoop::quit()
{
    quit_ = true;
    if (!isInLoopThread()) {
        wakeUp();
    }
}

void EventLoop::runInLoop(Functor cb)
{
    if (isInLoopThread()) {
        cb();
    } else {
        queueInLoop(std::move(cb));
    }
}

void EventLoop::queueInLoop(Functor cb)
{
    // 添加至任务队列
    {
        MutexGuard lock(mutex_);
        pendingFunctors_.push_back(std::move(cb));
    }

    // 若Loop为其他线程或者当前线程正在执行上层回调，则唤醒
    if (!isInLoopThread() || callingPendingFunctor_) {
        wakeUp();
    }
}

void EventLoop::wakeUp()
{
    uint64_t one = 1;
    ssize_t  n = ::write(wakeupFd_, &one, sizeof one);
    if (n != sizeof one) {
        LOG_ERROR << "EventLoop::wakeUp() writes " << n
                  << " bytes instead of 8";
    }
    LOG_TRACE << "EventLoop::wakeUp() writes 8 bytes";
}

void EventLoop::updateChannel(Channel* channel)
{
    assert(channel->ownerLoop() == this);
    assertInLoopThread();
    poller_->updateChannel(channel);
}
void EventLoop::removeChannel(Channel* channel)
{
    assert(channel->ownerLoop() == this);
    assertInLoopThread();
    poller_->removeChannel(channel);
}
bool EventLoop::hasChannel(Channel* channel)
{
    assert(channel->ownerLoop() == this);
    assertInLoopThread();
    return poller_->hasChannel(channel);
}
bool EventLoop::isInLoopThread() const
{
    return tid_ == CurrentThread::tid();
}
bool EventLoop::eventHandling() const
{
    return eventHandling_;
}
EventLoop* EventLoop::getEventLoopOfCurrentThread()
{
    return t_loopInThisThread;
}
Timestamp EventLoop::pollRetureTime() const
{
    return pollReturnTime_;
}
void EventLoop::assertInLoopThread()
{
    if (!isInLoopThread()) {
        abortNotInLoopThread();
    }
}

void EventLoop::handleRead()
{
    uint64_t one = 1;
    ssize_t  n = ::read(wakeupFd_, &one, sizeof(one));
    if (n != sizeof(one)) {
        LOG_ERROR << "EventLoop::wakeUp() reads " << n
                  << " bytes instead of 8";
    }
    LOG_TRACE << "EventLoop::wakeUp() reads 8 bytes";
}

void EventLoop::doPendingFunctors()
{
    std::vector<Functor> functors;
    callingPendingFunctor_ = true;

    {
        MutexGuard lock(mutex_);
        functors.swap(pendingFunctors_);
    }

    for (const auto& functor : functors) {
        functor();
    }

    callingPendingFunctor_ = false;
}

void EventLoop::abortNotInLoopThread()
{
    LOG_FATAL << "EventLoop::assertInLoopThread() - EventLoop" << this
              << " was created in thread id " << tid_
              << ", current thread id = " << CurrentThread::tid();
}

void EventLoop::printActiveChannels() const
{
    for (const Channel* Channel : activeChannels_) {
        LOG_INFO << "{" << Channel->reventsToString() << "}";
    }
}
