#pragma once

#include "topology.h"

class KnightTopology : public Topology {
public:
    explicit KnightTopology(std::vector<std::vector<bool>> initial_map);

    std::vector<Point> GetNeighbours(const Point& point) const override;
};
