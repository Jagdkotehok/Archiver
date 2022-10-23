#include <cstdint>
#include "overflow_topology.h"

OverflowTopology::OverflowTopology(std::vector<std::vector<bool>> initial_map) : Topology(initial_map) {
    CalcDistances();
}

std::vector<Point> OverflowTopology::GetNeighbours(const Point& point) const {
    std::vector<Point> result;
    std::vector<int> dx = {-1, 0, 0, 1};
    std::vector<int> dy = {0, -1, 1, 0};
    for (size_t direction = 0; direction < 4; ++direction) {
        int64_t new_x = static_cast<int64_t>(point.x) + dx[direction];
        int64_t new_y = static_cast<int64_t>(point.y) + dy[direction];
        int64_t width_i64 = static_cast<int64_t>(width);
        int64_t height_i64 = static_cast<int64_t>(height);

        if (new_x < 0) {
            new_x += width_i64;
        } else if (new_x >= width_i64) {
            new_x -= width_i64;
        }

        if (new_y < 0) {
            new_y += height_i64;
        } else if (new_y >= height_i64) {
            new_y -= height_i64;
        }

        Point neighbour{.x = static_cast<size_t>(new_x), .y = static_cast<size_t>(new_y)};
        if (map[neighbour.y][neighbour.x]) {
            continue;
        }  /// the way is blocked
        result.emplace_back(neighbour);
    }
    return result;
}
