#pragma once

#include <chrono>
#include <iostream>
#include <string>

namespace scl {

class Timer final {
public:
    explicit Timer(const std::string& title) : title_(title), start_time_(std::chrono::steady_clock::now()) {}

    ~Timer() {
        auto end_time = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time_).count();
        std::cout << title_ << " => " << "timer: " << duration << " ms." << std::endl;
    }

private:
    std::string title_;
    std::chrono::steady_clock::time_point start_time_;
};

}  // namespace scl
