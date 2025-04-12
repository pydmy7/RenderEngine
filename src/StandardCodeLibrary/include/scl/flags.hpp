#pragma once

#include <type_traits>

namespace scl {

template<typename T>
requires std::is_enum_v<T>
class Flags {
public:
    using underlying_type = std::underlying_type_t<T>;

    Flags() = default;

    explicit Flags(T value) : value_{static_cast<underlying_type>(value)} {}

    explicit Flags(std::underlying_type_t<T> value) : value_{value} {}

    Flags(const Flags&) = default;
    Flags(Flags&&) = default;

    Flags operator|(T o) const {
        return Flags{value_ | static_cast<underlying_type>(o)};
    }

    Flags operator&(T o) const {
        return Flags{value_ & static_cast<underlying_type>(o)};
    }

    Flags& operator=(const Flags&) = default;

    Flags& operator=(T value) {
        value_ = static_cast<underlying_type>(value);
        return *this;
    }

    Flags& operator|=(T o) {
        value_ |= static_cast<underlying_type>(o);
        return *this;
    }

    Flags& operator&=(T o) {
        value_ &= static_cast<underlying_type>(o);
        return *this;
    }

    Flags operator~() const noexcept { return ~value_; }

    explicit operator T() const { return static_cast<T>(value_); }

    explicit operator underlying_type() const { return value_; }

private:
    underlying_type value_{};
};

}  // namespace scl
