#pragma once

#include <variant>

namespace internal_types
{
struct MouseMoveEvent
{
    short deltaX;
    short deltaY;
};

enum class MouseScrollEvent
{
    Forward,
    Backward
};

enum class MouseKeyEvent
{
    LeftButtonPressed,
    LeftButtonUnpressed,
    RightButtonPressed,
    RightButtonUnpressed,
    MiddleButtonPressed,
    MiddleButtonUnpressed
};

struct MouseChangePositionEvent
{
    short x;
    short y;
};

using MouseEvent = std::variant<MouseMoveEvent, MouseScrollEvent, MouseKeyEvent, MouseChangePositionEvent>;

inline bool operator==(const MouseMoveEvent& lv, const MouseMoveEvent& rv)
{
    return (lv.deltaX == rv.deltaX && lv.deltaY == rv.deltaY);
}
} // namespace internal_types
