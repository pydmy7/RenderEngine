#include "scl/timer.hpp"

#include <iostream>

namespace scl {

Timer::Timer(const std::string& title) : title_(title), start_time_(std::chrono::steady_clock::now()) {}

Timer::~Timer() {
    auto end_time = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time_).count();
    std::cout << title_ << " => " << "timer: " << duration << " ms." << std::endl;
}

}  // namespace scl
