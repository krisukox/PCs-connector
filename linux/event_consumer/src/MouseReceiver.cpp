#include "event_consumer/MouseReceiver.hpp"
#include <X11/extensions/XTest.h>
#include <iostream>
#include <stdexcept>
#include "connection/Sender.hpp"

namespace event_consumer
{
MouseReceiver::MouseReceiver(Display* _display, std::unique_ptr<connection::Sender> _sender)
    : display{_display}, sender{std::move(_sender)}, cursorGuard{display}
{
}

MouseReceiver::~MouseReceiver() = default;

void MouseReceiver::onEvent(const internal_types::MouseEvent& mouseEvent)
{
    std::visit([this](const auto& event) { onEvent(event); }, mouseEvent);
}

void MouseReceiver::onEvent(const internal_types::MouseMoveEvent& mouseMoveEvent)
{
    if (cursorGuard.checkIfCursorOutOfScreen(mouseMoveEvent))
    {
        std::cout << "SEND OUT OF SCREEN" << std::endl;
        sender->send(internal_types::MouseChangePositionEvent{0, 0});
        // OUT OF SCREEN
    }
    XTestFakeRelativeMotionEvent(display, mouseMoveEvent.deltaX, mouseMoveEvent.deltaY, CurrentTime);
    XFlush(display);
}

void MouseReceiver::onEvent(const internal_types::MouseScrollEvent&) {}

void MouseReceiver::onEvent(const internal_types::MouseKeyEvent&) {}

void MouseReceiver::onEvent(const internal_types::MouseChangePositionEvent& event)
{
    XTestFakeRelativeMotionEvent(display, -10000, -10000, CurrentTime);
    XTestFakeRelativeMotionEvent(display, 1366 + event.x, event.y, CurrentTime);
    XFlush(display);
}

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
