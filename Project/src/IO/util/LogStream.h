#ifndef SCU_IO_UTIL_LOGSTREAM_H
#define SCU_IO_UTIL_LOGSTREAM_H
#include <SCU/IO/util/UnCopyable.h>
#include <cstring>
#include <string>
namespace SCU {
namespace IO {
namespace util {
namespace detail {
const int kSmallBuffer = 4000;
const int kLargerBuffer = 4000 * 1000;
/**
 * @brief 日志记录临时存放buffer
 *
 * @tparam SIZE
 */
template <int SIZE>
class FixedBuffer : UnCopyable {
public:
    FixedBuffer() : cur_(data_) {}

    void append(const char* buff, size_t len)
    {
        if (static_cast<size_t>(avail()) > len) {
            memcpy(cur_, buff, len);
            cur_ += len;
        }
    }

    void reset()
    {
        cur_ = data_;
    }

    void bzero()
    {
        memset(data_, 0, sizeof(data_));
    }

    std::string toString() const
    {
        return std::string(data_, length());
    }

    const char* data() const
    {
        return data_;
    }

    char* current()
    {
        return cur_;
    }

    int length() const
    {
        return static_cast<int>(cur_ - data_);
    }

    int avail() const
    {
        return static_cast<int>(end() - cur_);
    }

    void add(size_t len)
    {
        cur_ += len;
    }

private:
    const char* end() const
    {
        return data_ + sizeof(data_);
    }

private:
    char  data_[SIZE];
    char* cur_;
};
}  // namespace detail

class LogStream : UnCopyable {
public:
    using self = LogStream;
    using Buffer = detail::FixedBuffer<detail::kSmallBuffer>;

    self& operator<<(bool v);
    self& operator<<(char v);
    self& operator<<(short v);
    self& operator<<(unsigned short v);
    self& operator<<(int v);
    self& operator<<(unsigned int v);
    self& operator<<(long v);
    self& operator<<(unsigned long v);
    self& operator<<(long long v);
    self& operator<<(unsigned long long v);

    self& operator<<(float v);
    self& operator<<(double v);

    self& operator<<(const void*);
    self& operator<<(const char*);
    self& operator<<(const unsigned char*);
    self& operator<<(const std::string& v);

    const Buffer& buffer() const
    {
        return buffer_;
    }

private:
    template <typename T>
    void formatInteger(T);

    static const int kMaxNumericSize = 48;

    Buffer buffer_;
};

}  // namespace util
}  // namespace IO
}  // namespace SCU

#endif  // SCU_IO_UTIL_LOGSTREAM_H