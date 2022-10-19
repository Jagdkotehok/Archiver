#pragma once

#include "point.h"

#include <vector>

class Topology {
public:
    using Distance = ssize_t;

    std::vector<std::vector<bool>> map;

    size_t width;
    size_t height;

    std::vector<std::vector<Distance>> distance;

    size_t PointToIdx(const Point& p) const;
    Point IdxToPoint(const size_t& idx) const;

    explicit Topology(std::vector<std::vector<bool>> initial_map);

    void CalcDistances();

    virtual std::vector<Point> GetNeighbours(const Point& point) const;
    Distance MeasureDistance(const Point& from, const Point& to) const;

    static const Distance UNREACHABLE = -1;
};

