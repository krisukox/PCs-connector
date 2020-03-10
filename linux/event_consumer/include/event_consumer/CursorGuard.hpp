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

    void setContactPoints(const std::pair<internal_types::Point, internal_types::Point>&, const internal_types::Point&);

private:
    internal_types::Point getMouseCoordinate();

    Display* display;
    Window window;

    Window _w_;
    int _i_;
    unsigned _u_;
    int xCoordinate, yCoordinate;

    std::pair<internal_types::Point, internal_types::Point> contactPoints;
    internal_types::Point diffPoint;
};
} // namespace event_consumer
