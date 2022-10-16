//
// Created by Kotehok3 on 16.10.2022.
//

#include "point.h"

bool Point::operator==(const Point& other) const {
    return (x == other.x) && (y == other.y);
}
