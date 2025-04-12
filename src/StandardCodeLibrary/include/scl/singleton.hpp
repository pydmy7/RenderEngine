#pragma once

#include <cassert>
#include <memory>

namespace scl {

template<typename T, bool explicitInit>
class Singlton;

template<typename T>
class Singlton<T, false> {
public:
    static T& GetInstance() {
        static T instance;
        return instance;
    }
};

template<typename T>
class Singlton<T, true> {
public:
    static T& GetInstance() {
        assert(instance_ != nullptr);
        return *instance_;
    }

    template<typename... Args>
    static void init(Args&&... args) {
        instance_ = std::make_unique<T>(std::forward<Args>(args)...);
    }

    static void destroy() {
        instance_.reset();
        instance_ = nullptr;
    }

private:
    inline static std::unique_ptr<T> instance_;
};

}  // namespace scl
