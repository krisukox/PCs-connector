#include "event_consumer/MouseReceiver.hpp"
#include <X11/extensions/XTest.h>
#include <iostream>
#include <stdexcept>
#include "commons/CursorGuard.hpp"
#include "connection/Sender.hpp"
#include "internal_types/Visitor.hpp"

namespace
{
constexpr unsigned leftButton{1};
constexpr unsigned middleButton{2};
constexpr unsigned rightButton{3};

constexpr unsigned forwardScroll{4};
constexpr unsigned backwardScroll{5};

Window _w_;
int _i_;
unsigned _u_;

internal_types::Point getMouseCoordinate(Display* display, Window& window)
{
    int xCoordinate, yCoordinate;
    if (!XQueryPointer(display, window, &_w_, &_w_, &xCoordinate, &yCoordinate, &_i_, &_i_, &_u_))
    {
        throw std::runtime_error("wrong XQueryPointer result");
    }
    return {static_cast<short>(xCoordinate), static_cast<short>(yCoordinate)};
}
} // namespace

namespace event_consumer
{
MouseReceiver::MouseReceiver(
    Display* _display,
    std::unique_ptr<connection::Sender> _sender,
    std::shared_ptr<commons::CursorGuard> _cursorGuard)
    : display{_display}
    , window{XRootWindow(display, 0)}
    , sender{std::move(_sender)}
    , cursorGuard{_cursorGuard}
    , dispatchState{DispatchState::off}
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
    internal_types::Point cursor = getMouseCoordinate(display, window) + mouseMoveEvent;
    if (auto changeMousePositionEvent = cursorGuard->checkIfCursorOutOfScreen(cursor))
    {
        dispatchState = DispatchState::off;
        sender->send(changeMousePositionEvent.value());
        setCursorPosition(internal_types::MouseChangePositionEvent{2000, 2000});
    }
    XTestFakeRelativeMotionEvent(display, mouseMoveEvent.deltaX, mouseMoveEvent.deltaY, CurrentTime);
    XFlush(display);
}

void MouseReceiver::onEvent(const internal_types::MouseScrollEvent& mouseScrollEvent)
{
    switch (mouseScrollEvent)
    {
        case internal_types::MouseScrollEvent::Forward:
            XTestFakeButtonEvent(display, forwardScroll, True, CurrentTime);
            XTestFakeButtonEvent(display, forwardScroll, False, CurrentTime);
            break;
        case internal_types::MouseScrollEvent::Backward:
            XTestFakeButtonEvent(display, backwardScroll, True, CurrentTime);
            XTestFakeButtonEvent(display, backwardScroll, False, CurrentTime);
            break;
    }
    XFlush(display);
}

void MouseReceiver::onEvent(const internal_types::MouseKeyEvent& mouseKeyEvent)
{
    switch (mouseKeyEvent)
    {
        case internal_types::MouseKeyEvent::LeftButtonPressed:
            XTestFakeButtonEvent(display, leftButton, True, CurrentTime);
            break;
        case internal_types::MouseKeyEvent::LeftButtonUnpressed:
            XTestFakeButtonEvent(display, leftButton, False, CurrentTime);
            break;
        case internal_types::MouseKeyEvent::MiddleButtonPressed:
            XTestFakeButtonEvent(display, middleButton, True, CurrentTime);
            break;
        case internal_types::MouseKeyEvent::MiddleButtonUnpressed:
            XTestFakeButtonEvent(display, middleButton, False, CurrentTime);
            break;
        case internal_types::MouseKeyEvent::RightButtonPressed:
            XTestFakeButtonEvent(display, rightButton, True, CurrentTime);
            break;
        case internal_types::MouseKeyEvent::RightButtonUnpressed:
            XTestFakeButtonEvent(display, rightButton, False, CurrentTime);
            break;
    }
    XFlush(display);
}

void MouseReceiver::onEvent(const internal_types::MouseChangePositionEvent& event)
{
    dispatchState = DispatchState::on;
    setCursorPosition(event);
}

void MouseReceiver::setCursorPosition(const internal_types::MouseChangePositionEvent& event)
{
    XTestFakeRelativeMotionEvent(display, -10000, -10000, CurrentTime);
    XTestFakeRelativeMotionEvent(display, event.x, event.y, CurrentTime);
    XFlush(display);
}
} // namespace event_consumer
