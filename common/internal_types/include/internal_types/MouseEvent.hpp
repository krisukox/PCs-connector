#pragma once

#include <variant>

namespace internal_types
{
struct MouseMoveEvent
{
    short deltaX;
    short deltaY;
};

struct MouseScrollEvent
{
    short deltaForward;
    short deltaBackward;
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

inline bool operator==(const MouseScrollEvent& lv, const MouseScrollEvent& rv)
{
    return (lv.deltaForward == rv.deltaForward && lv.deltaBackward == rv.deltaBackward);
}
} // namespace internal_types
