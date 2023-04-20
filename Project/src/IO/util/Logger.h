#ifndef SCU_IO_UTIL_LOGGER_H
#define SCU_IO_UTIL_LOGGER_H
#include <SCU/IO/util/LogStream.h>
#include <SCU/IO/util/Timestamp.h>
#include <SCU/IO/util/UnCopyable.h>
#include <cstring>

namespace SCU {
namespace IO {
namespace util {
/**
 * @brief 日志系统：日志等级控制，信息控制（时间，函数），格式控制
 *
 */
class Logger : UnCopyable {
public:
    // 日志等级
    enum Level { TRACE, DEBUG, INFO, ERROR, WARN, FATAL, NUM_LOG_LEVELS };

    /**
     * @brief 标记日志对应文件名
     *
     */
    class SourceFile {
    public:
        template <int N>
        SourceFile(const char (&filename)[N])
            : data_(filename), size_(N - 1)
        {
            const char* slash = std::strchr(data_, '/');
            if (slash) {
                data_ = slash + 1;
                size_ -= static_cast<int>(data_ - filename);
            }
        }

        explicit SourceFile(const char* filename) : data_(filename)
        {
            const char* slash = std::strchr(data_, '/');
            if (slash) {
                data_ = slash + 1;
            }
            size_ = static_cast<int>(std::strlen(data_));
        }

        const char* data_;
        int         size_;
    };

    Logger(SourceFile file, int line);
    Logger(SourceFile file, int line, Level level);
    Logger(SourceFile file, int line, Level level, const char* func);
    Logger(SourceFile file, int line, bool toAbort);
    ~Logger();

    LogStream& stream()
    {
        return impl_.stream_;
    }

    static Level logLevel();
    static void  setLogLevel(Level level);

    typedef void (*OutputFunc)(const char* msg, int len);
    typedef void (*FlushFunc)();
    static void setOutput(OutputFunc out);
    static void setFlush(FlushFunc flush);

private:
    class Impl {
    public:
        typedef Logger::Level LogLevel;
        Impl(LogLevel level, int old_error, const SourceFile& file,
             int line);

        void finish();

        Timestamp  time_;
        LogStream  stream_;
        LogLevel   level_;
        int        line_;
        SourceFile basename_;
    };

    Impl impl_;
};

extern Logger::Level g_logLevel;

inline Logger::Level Logger::logLevel()
{
    return g_logLevel;
}

#define LOG_TRACE                                                          \
    if (SCU::IO::util::Logger::logLevel() <= SCU::IO::util::Logger::TRACE) \
    SCU::IO::util::Logger(__FILE__, __LINE__,                              \
                          SCU::IO::util::Logger::TRACE, __func__)          \
        .stream()
#define LOG_DEBUG                                                          \
    if (SCU::IO::util::Logger::logLevel() <= SCU::IO::util::Logger::DEBUG) \
    SCU::IO::util::Logger(__FILE__, __LINE__,                              \
                          SCU::IO::util::Logger::DEBUG, __func__)          \
        .stream()
#define LOG_INFO                                                           \
    if (SCU::IO::util::Logger::logLevel() <= SCU::IO::util::Logger::INFO)  \
    SCU::IO::util::Logger(__FILE__, __LINE__).stream()
#define LOG_WARN                                                           \
    nty::Logger(__FsLE__, __LINE__, SCU::IO::util::Logger::WARN).stream()
#define LOG_ERROR                                                          \
    SCU::IO::util::Logger(__FILE__, __LINE__,                              \
                          SCU::IO::util::Logger::ERROR)                    \
        .stream()
#define LOG_FATAL                                                          \
    SCU::IO::util::Logger(__FILE__, __LINE__,                              \
                          SCU::IO::util::Logger::FATAL)                    \
        .stream()
#define LOG_SYSERR SCU::IO::util::Logger(__FILE__, __LINE__, false).stream()
#define LOG_SYSFATAL                                                       \
    SCU::IO::util::Logger(__FILE__, __LINE__, true).stream()

extern const char* strerror_tl(int savedErrno);

}  // namespace util
}  // namespace IO
}  // namespace SCU

#endif  // SCU_IO_UTIL_LOGGER_H