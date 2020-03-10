#include "event_consumer/CursorGuard.hpp"

namespace event_consumer
{
CursorGuard::CursorGuard(Display* _display) : display{_display}, window{XRootWindow(display, 0)} {}

std::optional<internal_types::MouseChangePositionEvent> CursorGuard::checkIfCursorOutOfScreen(
    const internal_types::MouseMoveEvent& mouseMoveEvent)
{
    internal_types::Point newPoint = getMouseCoordinate() + mouseMoveEvent;
    if (newPoint.x < XWidthOfScreen(XDefaultScreenOfDisplay(display)))
    {
        return std::nullopt;
    }
    return internal_types::MouseChangePositionEvent{
        static_cast<short>(newPoint.x - XWidthOfScreen(XDefaultScreenOfDisplay(display))), newPoint.y};
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
} // namespace event_consumer
