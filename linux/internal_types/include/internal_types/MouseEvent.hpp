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
} // namespace internal_types
