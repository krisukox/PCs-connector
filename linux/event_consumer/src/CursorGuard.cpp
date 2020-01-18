#include "event_consumer/CursorGuard.hpp"

namespace event_consumer
{
CursorGuard::CursorGuard(Display* _display) : display{_display}, window{XRootWindow(display, 0)} {}

internal_types::Point CursorGuard::getMouseCoordinate()
{
    if (!XQueryPointer(display, window, &_w_, &_w_, &xCoordinate, &yCoordinate, &_i_, &_i_, &_u_))
    {
        throw std::runtime_error("wrong XQueryPointer result");
    }
    return {static_cast<short>(xCoordinate), static_cast<short>(yCoordinate)};
}

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
} // namespace event_consumer
