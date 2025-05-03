#include "scl/time.hpp"

namespace scl {

Time::Time()
    : last_frame_time_(std::chrono::system_clock::now())
    , delta_time_(std::chrono::system_clock::duration(0))
{
}

uint64_t Time::getCurrentTime() const {
    auto current_time = std::chrono::system_clock::now();
    return std::chrono::system_clock::to_time_t(current_time);
}

float Time::getDeltaTime() const {
    return std::chrono::duration_cast<std::chrono::milliseconds>(delta_time_).count() / 1000.0f;
}

uint32_t Time::getFPS() const {
    float delta = getDeltaTime();
    if (std::abs(delta) <= std::numeric_limits<float>::epsilon()) {
        return 10000;
    }
    return 1.0 / delta;
}

void Time::update() {
    auto current_time = std::chrono::system_clock::now();

    delta_time_ = current_time - last_frame_time_;
    last_frame_time_ = current_time;
}

}  // namespace scl
