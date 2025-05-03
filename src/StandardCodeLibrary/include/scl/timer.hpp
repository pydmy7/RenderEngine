#pragma once

#include <chrono>
#include <string>

namespace scl {

class Timer final {
public:
    explicit Timer(const std::string& title);
    ~Timer();

private:
    std::string title_;
    std::chrono::steady_clock::time_point start_time_;
};

}  // namespace scl
