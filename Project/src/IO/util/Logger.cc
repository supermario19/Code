#include <SCU/IO/util/CurrentThread.h>
#include <SCU/IO/util/Logger.h>
#include <SCU/IO/util/Timestamp.h>

namespace SCU {
namespace IO {
namespace util {

namespace CurrentThread {
__thread char t_errnoBuf[512] = {0};
__thread char t_time[64] = {0};
}  // namespace CurrentThread

Logger::Level g_logLevel = Logger::TRACE;

const char* LogLevelName[Logger::NUM_LOG_LEVELS] = {
    "TRACE ", "DEBUG ", "INFO  ", "WARN  ", "ERROR ", "FATAL ",
};

const char* strerror_tl(int savedErrno)
{
    return strerror_r(savedErrno, CurrentThread::t_errnoBuf,
                      sizeof CurrentThread::t_errnoBuf);
}

void defaultOutput(const char* msg, int len)
{
    size_t n = fwrite(msg, 1, len, stdout);
    (void)n;
}

void defaultFlush()
{
    fflush(stdout);
}

Logger::Level defaultLogLevel()
{
    return Logger::INFO;
}

Logger::OutputFunc g_output = defaultOutput;
Logger::FlushFunc  g_flush = defaultFlush;

}  // namespace util
}  // namespace IO
}  // namespace SCU

using namespace SCU::IO::util;

Logger::Logger(SourceFile file, int line) : impl_(INFO, 0, file, line) {}

Logger::Logger(SourceFile file, int line, Level level)
    : impl_(level, 0, file, line)
{
}
Logger::Logger(SourceFile file, int line, Level level, const char* func)
    : impl_(level, 0, file, line)
{
    impl_.stream_ << func << ' ';
}

Logger::Logger(SourceFile file, int line, bool toAbort)
    : impl_(toAbort ? FATAL : ERROR, errno, file, line)
{
}

Logger::~Logger()
{
    impl_.finish();
    const LogStream::Buffer& buf(stream().buffer());

    g_output(buf.data(), buf.length());
    if (impl_.level_ == FATAL) {
        g_flush();
        abort();
    }
}

Logger::Impl::Impl(LogLevel level, int savedErrno,
                   const SourceFile& filename, int line)
    : time_(Timestamp::now()),
      stream_(),
      level_(level),
      line_(line),
      basename_(filename)
{
    // 记录当前时间
    stream_ << Timestamp::now().toFormatString();

    // 记录当前线程tid
    stream_ << " tid = " << CurrentThread::tidString();

    // 输出日志等级
    stream_ << " " << LogLevelName[level_];

    // 输出errno
    if (savedErrno != 0) {
        stream_ << strerror_tl(savedErrno) << "(errno = " << savedErrno
                << ")";
    }
}

void Logger::Impl::finish()
{
    stream_ << " - " << basename_.data_ << ":" << line_ << '\n';
}

void Logger::setOutput(OutputFunc out)
{
    g_output = out;
}

void Logger::setFlush(FlushFunc flush)
{
    g_flush = flush;
}

void Logger::setLogLevel(Logger::Level level)
{
    g_logLevel = level;
}