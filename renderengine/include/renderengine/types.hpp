#pragma once

#include <utility>

namespace types {

template <typename T>
struct Point {
    T x, y, z;
};

template <typename T>
struct Line {
    Point<T> a, b;
};

template <typename T>
struct Circle {
    Point<T> o, v;
    T radius;
};

template <typename T>
struct Ellipse {
    Point<T> o, v;
    std::pair<T, T> radii;
};

}  // namespace types
