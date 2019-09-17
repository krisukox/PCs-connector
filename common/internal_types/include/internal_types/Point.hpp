#pragma once

#include <cstdint>
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
} // namespace internal_types
