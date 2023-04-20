#include <SCU/IO/util/LogStream.h>
#include <algorithm>
namespace SCU {
namespace IO {
namespace util {
namespace detail {
const char  digits[] = "9876543210123456789";
const char* zero = digits + 9;
static_assert(sizeof(digits) == 20, "wrong number of digits");

const char digitsHex[] = "0123456789ABCDEF";
static_assert(sizeof digitsHex == 17, "wrong number of digitsHex");

// int转为字符串（10进制）
template <typename T>
size_t convert(char buf[], T value)
{
    T     i = value;
    char* p = buf;

    do {
        int lsd = static_cast<int>(i % 10);
        i /= 10;
        *p++ = zero[lsd];
    } while (i != 0);

    if (value < 0) {
        *p++ = '-';
    }
    *p = '\0';
    std::reverse(buf, p);

    return p - buf;
}

// // int转为字符串（16进制）
size_t convertHex(char buf[], uintptr_t value)
{
    uintptr_t i = value;
    char*     p = buf;

    do {
        int lsd = static_cast<int>(i % 16);
        i /= 16;
        *p++ = digitsHex[lsd];
    } while (i != 0);

    *p = '\0';
    std::reverse(buf, p);

    return p - buf;
}

template class FixedBuffer<kSmallBuffer>;
template class FixedBuffer<kLargerBuffer>;

}  // namespace detail
}  // namespace util
}  // namespace IO
}  // namespace SCU

using namespace SCU::IO::util;

template <typename T>
void LogStream::formatInteger(T v)
{
    if (buffer_.avail() >= kMaxNumericSize) {
        size_t len = detail::convert(buffer_.current(), v);
        buffer_.add(len);
    }
}

LogStream::self& LogStream::operator<<(bool v)
{
    buffer_.append(v == true ? "1" : "0", 1);
    return *this;
}

LogStream::self& LogStream::operator<<(char v)
{
    buffer_.append(&v, sizeof(char));
    return *this;
}
LogStream::self& LogStream::operator<<(short v)
{
    *this << static_cast<int>(v);
    return *this;
}
LogStream::self& LogStream::operator<<(unsigned short v)
{
    *this << static_cast<unsigned int>(v);
    return *this;
}
LogStream::self& LogStream::operator<<(int v)
{
    formatInteger(v);
    return *this;
}
LogStream::self& LogStream::operator<<(unsigned int v)
{
    formatInteger(v);
    return *this;
}
LogStream::self& LogStream::operator<<(long v)
{
    formatInteger(v);
    return *this;
}
LogStream::self& LogStream::operator<<(unsigned long v)
{
    formatInteger(v);
    return *this;
}
LogStream::self& LogStream::operator<<(long long v)
{
    formatInteger(v);
    return *this;
}
LogStream::self& LogStream::operator<<(unsigned long long v)
{
    formatInteger(v);
    return *this;
}

LogStream::self& LogStream::operator<<(float v)
{
    *this << static_cast<double>(v);
    return *this;
}
LogStream::self& LogStream::operator<<(double v)
{
    if (buffer_.avail() >= kMaxNumericSize) {
        int len = snprintf(buffer_.current(), kMaxNumericSize, "%.12g", v);
        buffer_.add(len);
    }
    return *this;
}

LogStream::self& LogStream::operator<<(const void* p)
{
    uintptr_t v = reinterpret_cast<uintptr_t>(p);
    if (buffer_.avail() >= kMaxNumericSize) {
        char* buf = buffer_.current();
        buf[0] = '0';
        buf[1] = 'x';
        size_t len = detail::convertHex(buf + 2, v);
        buffer_.add(len + 2);
    }
    return *this;
}

LogStream::self& LogStream::operator<<(const char* str)
{
    if (str) {
        buffer_.append(str, strlen(str));
    } else {
        buffer_.append("(null)", 6);
    }
    return *this;
}

LogStream::self& LogStream::operator<<(const unsigned char* str)
{
    *this << reinterpret_cast<const char*>(str);
    return *this;
}

LogStream::self& LogStream::operator<<(const std::string& v)
{
    buffer_.append(v.data(), v.size());
    return *this;
}