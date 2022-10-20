#pragma once

#include <cstddef>
#include <functional>

struct Point {
    size_t x = 0;
    size_t y = 0;

    bool operator==(const Point& other) const {
        return (x == other.x) && (y == other.y);
    }
};

namespace std {
template <>
class hash<Point> {
public:
    size_t operator()(const Point& point) const {
        return ((point.x << 16) + point.y) ^ 14042004;
    }
};
}  // namespace std
