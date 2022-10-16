#include "robot.h"

robot::Path robot::FindPath(World& world) {
    std::vector<Point> result;
    Point start = world.start_;
    Point end = world.end_;
    if (world.topology_.MeasureDistance(start, end) == Topology::UNREACHABLE) {
        return result;
    }
    result.emplace_back(start);
    while (world.current_position_ != end) {
        Point next_step;
        Topology::Distance minimal_distance = Topology::UNREACHABLE;
        for (const auto& [neighbour, distance] : world.Lookup()) {
            if (minimal_distance == Topology::UNREACHABLE || minimal_distance > distance) {
                minimal_distance = distance;
                next_step = neighbour;
            }
        }
        result.emplace_back(next_step);
        world.Move(next_step);
    }
    return result;
}