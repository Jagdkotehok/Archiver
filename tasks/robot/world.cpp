#include "world.h"
World::World(const Topology& topology, Point start, Point end)
    : start_(start), end_(end), current_position_(start), topology_(topology) {
}

std::unordered_map<Point, Topology::Distance> World::Lookup() const {
    std::unordered_map<Point, Topology::Distance> result;
    for (const Point& neighbour : topology_.GetNeighbours(current_position_)) {
        result[neighbour] = topology_.MeasureDistance(neighbour, end_);
    }
    return result;
}

const Point& World::GetStart() const {
    return start_;
}

const Point& World::GetEnd() const {
    return end_;
}

const Point& World::GetCurrentPosition() const {
    return current_position_;
}

void World::Move(const Point& to) {
    for (const Point& neighbour : topology_.GetNeighbours(GetCurrentPosition())) {
        if (neighbour == to) {
            current_position_ = to;
            return;
        }
    }
    throw IllegalMoveException();
}
