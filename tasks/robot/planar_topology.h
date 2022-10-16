#pragma once

#include "topology.h"

class PlanarTopology : public Topology {
public:
    explicit PlanarTopology(std::vector<std::vector<bool>> initial_map);

    std::vector<Point> GetNeighbours(const Point& point) const override;
};
