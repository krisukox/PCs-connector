#include "event_consumer/MouseReceiver.hpp"
#include <X11/extensions/XTest.h>
#include <stdexcept>

namespace event_consumer
{
MouseReceiver::MouseReceiver(Display* _display) : display{_display}, cursorGuard{display} {}

MouseReceiver::~MouseReceiver() = default;

void MouseReceiver::onEvent(const internal_types::MouseEvent& mouseEvent)
{
    std::visit([this](const auto& event) { onEvent(event); }, mouseEvent);
}

void MouseReceiver::onEvent(const internal_types::MouseMoveEvent& mouseMoveEvent)
{
    if (cursorGuard.checkIfCursorOutOfScreen(mouseMoveEvent))
    {
        // OUT OF SCREEN
    }
    XTestFakeRelativeMotionEvent(display, mouseMoveEvent.deltaX, mouseMoveEvent.deltaY, CurrentTime);
    XFlush(display);
}

void MouseReceiver::onEvent(const internal_types::MouseScrollEvent&) {}

void MouseReceiver::onEvent(const internal_types::MouseKeyEvent&) {}

MouseReceiver::CursorGuard::CursorGuard(Display* _display) : display{_display}, window{XRootWindow(display, 0)} {}

internal_types::Point MouseReceiver::CursorGuard::getMouseXCoordinate()
{
    if (!XQueryPointer(display, window, &_w_, &_w_, &xCoordinate, &yCoordinate, &_i_, &_i_, &_u_))
    {
        throw std::runtime_error("wrong XQueryPointer result");
    }
    return {static_cast<short>(xCoordinate), static_cast<short>(yCoordinate)};
}

bool MouseReceiver::CursorGuard::checkIfCursorOutOfScreen(const internal_types::MouseMoveEvent& mouseMoveEvent)
{
    internal_types::Point newPoint = getMouseXCoordinate() + mouseMoveEvent;
    return newPoint.x >= XWidthOfScreen(XDefaultScreenOfDisplay(display));
}
} // namespace event_consumer
