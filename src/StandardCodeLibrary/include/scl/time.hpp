#pragma once

#include <chrono>

namespace scl {

class Time final {
public:
    explicit Time();
    uint64_t getCurrentTime() const;
    float getDeltaTime() const;
    uint32_t getFPS() const;
    void update();

private:
    std::chrono::system_clock::time_point last_frame_time_;
    std::chrono::system_clock::duration delta_time_;
};

}  // namespace scl
