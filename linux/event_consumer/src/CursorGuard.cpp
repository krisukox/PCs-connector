#include "event_consumer/CursorGuard.hpp"
#include <iostream>

namespace event_consumer
{
CursorGuard::CursorGuard(Display* _display) : display{_display}, window{XRootWindow(display, 0)} {}

std::optional<internal_types::MouseChangePositionEvent> CursorGuard::checkIfCursorOutOfScreen(
    const internal_types::MouseMoveEvent& mouseMoveEvent)
{
    internal_types::Point newPoint = getMouseCoordinate() + mouseMoveEvent;
    if (isCursorInsideScreen(newPoint) || isCursorOutOfContactArea(newPoint))
    {
        return std::nullopt;
    }
    std::cout << newPoint.x << " " << diffPoint.x << " " << static_cast<short>(newPoint.x + diffPoint.x) << std::endl;
    return internal_types::MouseChangePositionEvent{static_cast<short>(newPoint.x + diffPoint.x),
                                                    static_cast<short>(newPoint.y + diffPoint.y)};
}

void CursorGuard::setContactPoints(
    const std::pair<internal_types::Point, internal_types::Point>& contactPoints_,
    const internal_types::Point& diffPoint_)
{
    contactPoints = contactPoints_;
    diffPoint = diffPoint_;
}

internal_types::Point CursorGuard::getMouseCoordinate()
{
    if (!XQueryPointer(display, window, &_w_, &_w_, &xCoordinate, &yCoordinate, &_i_, &_i_, &_u_))
    {
        throw std::runtime_error("wrong XQueryPointer result");
    }
    return {static_cast<short>(xCoordinate), static_cast<short>(yCoordinate)};
}

bool CursorGuard::isCursorInsideScreen(const internal_types::Point& cursor)
{
    return cursor.x < XWidthOfScreen(XDefaultScreenOfDisplay(display)) && cursor.x >= 0 &&
        cursor.y < XHeightOfScreen(XDefaultScreenOfDisplay(display)) && cursor.y >= 0;
}

bool CursorGuard::isCursorOutOfContactArea(const internal_types::Point& cursor)
{
    std::cout << contactPoints.first.x << " " << contactPoints.first.y << " " << contactPoints.second.x << " "
              << contactPoints.second.y << std::endl;
    if (contactPoints.first.x == contactPoints.second.x)
    {
        if (contactPoints.first.y < contactPoints.second.y)
        {
            return cursor.y < contactPoints.first.y || cursor.y > contactPoints.second.y;
        }
        return cursor.y > contactPoints.first.y || cursor.y < contactPoints.second.y;
    }
    if (contactPoints.first.x < contactPoints.second.x)
    {
        return cursor.x < contactPoints.first.x || cursor.x > contactPoints.second.x;
    }
    return cursor.x > contactPoints.first.x || cursor.x < contactPoints.second.x;
}
} // namespace event_consumer
