#include "checkers_topology.h"
#include <unordered_set>
#include <queue>
#include <cstdint>

CheckersTopology::CheckersTopology(std::vector<std::vector<bool>> initial_map) : Topology(initial_map) {
    CalcDistances();
}

std::vector<Point> CheckersTopology::GetNeighbours(const Point& point) const {
    std::vector<int> dx_step = {-2, -2, 2, 2};
    std::vector<int> dy_step = {-2, 2, -2, 2};

    std::vector<int> dx_occ = {-1, -1, 1, 1};
    std::vector<int> dy_occ = {-1, 1, -1, 1};
    /// offsets to check if there is blocked cell

    std::unordered_set<Point> visited;
    std::queue<Point> points_to_visit;

    points_to_visit.emplace(point);
    visited.emplace(point);

    while (!points_to_visit.empty()) {
        Point cur_point = points_to_visit.front();
        points_to_visit.pop();
        for (size_t direction = 0; direction < 4; ++direction) {
            int64_t x2 = static_cast<int64_t>(cur_point.x) + dx_step[direction];
            int64_t y2 = static_cast<int64_t>(cur_point.y) + dy_step[direction];

            int64_t x1 = static_cast<int64_t>(cur_point.x) + dx_occ[direction];
            int64_t y1 = static_cast<int64_t>(cur_point.y) + dy_occ[direction];

            int64_t width_i64 = static_cast<int64_t>(width);
            int64_t height_i64 = static_cast<int64_t>(height);

            if (!(-1 < x2 && x2 < width_i64)) {
                continue;
            }  /// neighbour is out of bound

            if (!(-1 < y2 && y2 < height_i64)) {
                continue;
            }  /// neighbour is out of bound

            Point neighbour{.x = static_cast<size_t>(x2), .y = static_cast<size_t>(y2)};

            if (map[y2][x2]) {
                continue;
            }  /// neighbouring cell is occupied

            if (!map[y1][x1]) {
                continue;
            }  /// cell between start and destination point is not occupied

            if (visited.find(neighbour) == visited.end()) {
                visited.emplace(neighbour);
                points_to_visit.emplace(neighbour);
            }
        }
    }  /// case 1 jumping over obstacles

    for (size_t direction = 0; direction < 4; ++direction) {
        int64_t x1 = static_cast<int64_t>(point.x) + dx_occ[direction];
        int64_t y1 = static_cast<int64_t>(point.y) + dy_occ[direction];

        int64_t width_i64 = static_cast<int64_t>(width);
        int64_t height_i64 = static_cast<int64_t>(height);

        if (!(-1 < x1 && x1 < width_i64)) {
            continue;
        }  /// neighbour is out of bound

        if (!(-1 < y1 && y1 < height_i64)) {
            continue;
        }  /// neighbour is out of bound

        Point neighbour{.x = static_cast<size_t>(x1), .y = static_cast<size_t>(y1)};

        if (map[y1][x1]) {
            continue;
        }  /// cell between start and destination point is not occupied

        if (visited.find(neighbour) == visited.end()) {
            visited.emplace(neighbour);
            points_to_visit.emplace(neighbour);
        }
    }  /// case 2 jumping without obstacles bypassing

    std::vector<Point> result;
    for (const Point& p : visited) {
        result.emplace_back(p);
    }

    return result;

}

