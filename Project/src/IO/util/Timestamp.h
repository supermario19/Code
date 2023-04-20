#ifndef SCU_IO_UTIL_TIMESTAMP_H
#define SCU_IO_UTIL_TIMESTAMP_H
#include <SCU/IO/util/Copyable.h>
#include <chrono>
#include <string>
#include <boost/operators.hpp>

namespace SCU {
namespace IO {
namespace util {
class Timestamp : public Copyable,
                  public boost::less_than_comparable<Timestamp>,
                  public boost::equality_comparable<Timestamp> {
public:
    Timestamp() : microSecondsSinceEpoch_(0) {}

    explicit Timestamp(std::time_t microSecondsSinceEpoch)
        : microSecondsSinceEpoch_(microSecondsSinceEpoch)
    {
    }

    bool valid() const
    {
        return microSecondsSinceEpoch_ == 0;
    }

    std::time_t microSecondsSinceEpoch() const
    {
        return microSecondsSinceEpoch_;
    }

    std::string toString() const;

    std::string toFormatString() const;

public:
    static Timestamp now();

    static Timestamp invalid();

private:
    std::time_t microSecondsSinceEpoch_;
};

inline bool operator<(const Timestamp& lhs, const Timestamp& rhs)
{
    return lhs.microSecondsSinceEpoch() < rhs.microSecondsSinceEpoch();
}

inline bool operator==(const Timestamp& lhs, const Timestamp& rhs)
{
    return lhs.microSecondsSinceEpoch() == rhs.microSecondsSinceEpoch();
}

}  // namespace util
}  // namespace IO
}  // namespace SCU

#endif  // SCU_IO_UTIL_TIMESTAMP_H
