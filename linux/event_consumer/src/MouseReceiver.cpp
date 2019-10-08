#include "event_consumer/MouseReceiver.hpp"
#include <X11/extensions/XTest.h>
#include <iostream>
#include <stdexcept>
#include "connection/Sender.hpp"
#include "internal_types/Visitor.hpp"

namespace event_consumer
{
MouseReceiver::MouseReceiver(Display* _display, std::unique_ptr<connection::Sender> _sender)
    : display{_display}
    , sender{std::move(_sender)}
    , cursorGuard{display}
    , dispatchState{DispatchState::off}
    , screen{DefaultScreenOfDisplay(display)}
{
}

MouseReceiver::~MouseReceiver() = default;

void MouseReceiver::onEvent(const internal_types::MouseEvent& mouseEvent)
{
    std::visit(
        internal_types::Visitor{[this](const internal_types::MouseChangePositionEvent& event) { onEvent(event); },
                                [this](const auto& event) {
                                    if (dispatchState == DispatchState::on)
                                    {
                                        onEvent(event);
                                    }
                                }},
        mouseEvent);
}

void MouseReceiver::onEvent(const internal_types::MouseMoveEvent& mouseMoveEvent)
{
    if (auto changeMousePositionEvent = cursorGuard.checkIfCursorOutOfScreen(mouseMoveEvent))
    {
        dispatchState = DispatchState::off;
        sender->send(changeMousePositionEvent.value());
        setCursorPosition(internal_types::MouseChangePositionEvent{2000, 2000});
    }
    XTestFakeRelativeMotionEvent(display, mouseMoveEvent.deltaX, mouseMoveEvent.deltaY, CurrentTime);
    XFlush(display);
}

void MouseReceiver::onEvent(const internal_types::MouseScrollEvent&) {}

void MouseReceiver::onEvent(const internal_types::MouseKeyEvent&) {}

void MouseReceiver::onEvent(const internal_types::MouseChangePositionEvent& event)
{
    dispatchState = DispatchState::on;
    setCursorPosition(changeToRelative(event));
}

void MouseReceiver::setCursorPosition(const internal_types::MouseChangePositionEvent& event)
{
    XTestFakeRelativeMotionEvent(display, -10000, -10000, CurrentTime);
    XTestFakeRelativeMotionEvent(display, event.x, event.y, CurrentTime);
    XFlush(display);
}

internal_types::MouseChangePositionEvent MouseReceiver::changeToRelative(
    const internal_types::MouseChangePositionEvent& event)
{
    auto returnEvent = event;
    returnEvent.x += screen->width;
    return returnEvent;
}
} // namespace event_consumer
