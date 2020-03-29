#pragma once

#include <cstdint>
#include <ostream>
#include "MouseEvent.hpp"

namespace internal_types
{
struct Point
{
    short x;
    short y;
};

inline Point operator+(const Point& point, const MouseMoveEvent& mouseMoveEvent)
{
    return {static_cast<short>(point.x + mouseMoveEvent.deltaX), static_cast<short>(point.y + mouseMoveEvent.deltaY)};
}

inline Point operator+(const Point& lvPoint, const Point& rvPoint)
{
    return {static_cast<short>(lvPoint.x + rvPoint.x), static_cast<short>(lvPoint.y + rvPoint.y)};
}

inline std::ostream& operator<<(std::ostream& os, const Point& point)
{
    os << "x: " << point.x << " y: " << point.y;
    return os;
}
} // namespace internal_types
