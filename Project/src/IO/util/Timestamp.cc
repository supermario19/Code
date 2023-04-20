#include <SCU/IO/util/Timestamp.h>

using namespace SCU::IO::util;
using namespace std::chrono;

Timestamp Timestamp::now()
{
    // 获取操作系统当前时间点（精确到微秒）
    time_point<system_clock, microseconds> tpMicro =
        time_point_cast<microseconds>(system_clock::now());
    // (微秒精度的)时间点 => (微秒精度的)时间戳
    time_t totalMicroSeconds = tpMicro.time_since_epoch().count();

    return Timestamp(totalMicroSeconds / (1000 * 1000));
}

Timestamp Timestamp::invalid()
{
    return Timestamp(0);
}

std::string Timestamp::toString() const
{
    std::tm* tm = localtime(&microSecondsSinceEpoch_);
    char*    buffer = asctime(tm);
    return buffer;
}

std::string Timestamp::toFormatString() const
{
    std::tm* tm = localtime(&microSecondsSinceEpoch_);
    char     buffer[64] = {0};
    snprintf(buffer, 64, "%04d-%02d-%02d %02d:%02d:%02d",
             tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour,
             tm->tm_min, tm->tm_sec);
    return buffer;
}