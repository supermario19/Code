#ifndef SCU_IO_UTIL_UNCOPYABLE_H
#define SCU_IO_UTIL_UNCOPYABLE_H

class UnCopyable {
public:
    UnCopyable() = default;
    ~UnCopyable() = default;
    UnCopyable(const UnCopyable &) = delete;
    UnCopyable& operator=(const UnCopyable &) = delete;
};

#endif  // SCU_IO_UTIL_UNCOPYABLE_H