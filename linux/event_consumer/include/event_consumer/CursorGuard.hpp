#pragma once

#include <X11/Xlib.h>
#include <optional>
#include "internal_types/Point.hpp"

namespace event_consumer
{
class CursorGuard
{
public:
    CursorGuard(Display*);
    std::optional<internal_types::MouseChangePositionEvent> checkIfCursorOutOfScreen(
        const internal_types::MouseMoveEvent&);

private:
    internal_types::Point getMouseCoordinate();

    Display* display;
    Window window;

    Window _w_;
    int _i_;
    unsigned _u_;
    int xCoordinate, yCoordinate;
};
} // namespace event_consumer
