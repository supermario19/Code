#ifndef SCU_IO_UTIL_COPYABLE_H
#define SCU_IO_UTIL_COPYABLE_H

class Copyable {
public:
    Copyable() = default;
    ~Copyable() = default;
    Copyable(const Copyable &) = default;
    Copyable& operator=(const Copyable &) = default;
};

#endif  // SCU_IO_UTIL_COPYABLE_H