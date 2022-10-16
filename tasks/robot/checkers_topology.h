#pragma once

#include "topology.h"

class CheckersTopology : public Topology {
public:
    explicit CheckersTopology(std::vector<std::vector<bool>> initial_map);

    std::vector<Point> GetNeighbours(const Point& point) const override;
};
