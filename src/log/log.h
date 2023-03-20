#pragma once


#include <deque>
#include <string>
#include <mutex>
#include <cstring>
#include <iostream>
#include <syncstream>


extern "C" bool log_get_message(char* buf, size_t len);


class Log final {
    friend bool log_get_message(char* buf, size_t len);
private:
    static inline std::stringstream _stream;
public:
    Log() = default;
    Log(const Log& other) = delete;
    Log& operator=(const Log& other) = delete;

    template <typename T>
    Log& operator<<(const T& msg) {
        std::cout << msg;
        _stream << msg;
        return *this;
    }

    Log& operator<<(const std::stringstream& sstr) {
        std::cout << sstr.str();
        _stream << sstr.str();
        return *this;
    }
};

