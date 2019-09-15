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

using MouseEvent = std::variant<MouseMoveEvent, MouseScrollEvent, MouseKeyEvent>;

inline bool operator==(const MouseMoveEvent& lv, const MouseMoveEvent& rv)
{
    return (lv.deltaX == rv.deltaX && lv.deltaY == rv.deltaY);
}

inline bool operator==(const MouseScrollEvent& lv, const MouseScrollEvent& rv)
{
    return (lv.deltaForward == rv.deltaForward && lv.deltaForward == rv.deltaForward);
}
} // namespace internal_types
