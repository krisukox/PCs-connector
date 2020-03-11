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
    internal_types::MouseChangePositionEvent changeToRelative(const internal_types::MouseChangePositionEvent&);

private:
    internal_types::Point getMouseCoordinate();
    bool isCursorInsideScreen(const internal_types::Point&);
    bool isCursorOutOfContactArea(const internal_types::Point&);

    Display* display;
    Window window;
    Screen* screen;

    std::pair<internal_types::Point, internal_types::Point> contactPoints;
    internal_types::Point diffPoint;

    Window _w_;
    int _i_;
    unsigned _u_;
    int xCoordinate, yCoordinate;
};
} // namespace event_consumer
