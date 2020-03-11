#include "event_consumer/CursorGuard.hpp"

namespace event_consumer
{
CursorGuard::CursorGuard(Display* _display)
    : display{_display}, window{XRootWindow(display, 0)}, screen{XDefaultScreenOfDisplay(display)}
{
}

std::optional<internal_types::MouseChangePositionEvent> CursorGuard::checkIfCursorOutOfScreen(
    const internal_types::MouseMoveEvent& mouseMoveEvent)
{
    internal_types::Point newPoint = getMouseCoordinate() + mouseMoveEvent;
    if (isCursorInsideScreen(newPoint) || isCursorOutOfContactArea(newPoint))
    {
        return std::nullopt;
    }
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

internal_types::MouseChangePositionEvent CursorGuard::changeToRelative(
    const internal_types::MouseChangePositionEvent& event)
{
    auto returnEvent = event;
    returnEvent.x += screen->width;
    return returnEvent;
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
    return cursor.x < XWidthOfScreen(screen) && cursor.x >= 0 && cursor.y < XHeightOfScreen(screen) && cursor.y >= 0;
}

bool CursorGuard::isCursorOutOfContactArea(const internal_types::Point& cursor)
{
    if (contactPoints.first.x == contactPoints.second.x)
    {
        if (contactPoints.first.x == 0)
        {
            if (cursor.x > 0)
            {
                return true;
            }
        }
        else if (cursor.x < 0)
        {
            return true;
        }
        if (contactPoints.first.y < contactPoints.second.y)
        {
            return cursor.y < contactPoints.first.y || cursor.y > contactPoints.second.y;
        }
        return cursor.y > contactPoints.first.y || cursor.y < contactPoints.second.y;
    }
    if (contactPoints.first.y == 0)
    {
        if (cursor.y > 0)
        {
            return true;
        }
    }
    else if (cursor.y < 0)
    {
        return true;
    }
    if (contactPoints.first.x < contactPoints.second.x)
    {
        return cursor.x < contactPoints.first.x || cursor.x > contactPoints.second.x;
    }
    return cursor.x > contactPoints.first.x || cursor.x < contactPoints.second.x;
}
} // namespace event_consumer
