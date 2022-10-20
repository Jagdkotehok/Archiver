#include "topology.h"
#include <iostream>

const Topology::Distance Topology::UNREACHABLE;

Topology::Topology(std::vector<std::vector<bool>> initial_map) {
    map = initial_map;
    height = map.size();
    if (!map.empty()) {
        width = map[0].size();
    } else {
        width = 0;
    }
}

void Topology::CalcDistances() {
    size_t total_vertices = height * width;
    distance.assign(total_vertices, std::vector<Distance>(total_vertices, UNREACHABLE));
    for (size_t i = 0; i < total_vertices; ++i) {
        Point cur_point = IdxToPoint(i);
        for (const Point& neighbour : GetNeighbours(cur_point)) {
            distance[i][PointToIdx(neighbour)] = 1;
        }
        distance[i][i] = 0;
    }
    /// relaxing paths
    for (size_t k = 0; k < total_vertices; ++k) {
        for (size_t i = 0; i < total_vertices; ++i) {
            for (size_t j = 0; j < total_vertices; ++j) {
                if (distance[i][k] != UNREACHABLE && distance[k][j] != UNREACHABLE) {
                    if (distance[i][j] == UNREACHABLE || distance[i][j] > distance[i][k] + distance[k][j]) {
                        distance[i][j] = distance[i][k] + distance[k][j];
                    }
                }
            }
        }
    }
}

std::vector<Point> Topology::GetNeighbours(const Point& point) const {
    return std::vector<Point>();
}

Topology::Distance Topology::MeasureDistance(const Point& from, const Point& to) const {
    return distance[PointToIdx(from)][PointToIdx(to)];
}

size_t Topology::PointToIdx(const Point& p) const {
    return p.y * width + p.x;
}

Point Topology::IdxToPoint(const size_t& idx) const {
    return Point{.x = idx % width, .y = idx / width};
}
