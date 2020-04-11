#include "event_consumer/MouseReceiver.hpp"
#include <X11/extensions/XTest.h>
#include <iostream>
#include <stdexcept>
#include "commons/CursorGuard.hpp"
#include "connection/Sender.hpp"
#include "gui/CursorManagement.h"
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
MouseReceiver::MouseReceiver(Display* _display, std::unique_ptr<commons::CursorGuard> _cursorGuard)
    : display{_display}
    , window{XRootWindow(display, 0)}
    , cursorGuard{std::move(_cursorGuard)}
    , dispatchState{DispatchState::off}
{
}

void MouseReceiver::start(ForwardEvent _forwardEvent)
{
    forwardEvent = _forwardEvent;
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
    internal_types::Point cursor = CursorManagement::getPosition() + mouseMoveEvent;
    if (auto changeMousePositionEvent = cursorGuard->checkIfCursorOutOfScreen(cursor))
    {
        forwardEvent(changeMousePositionEvent.value());
        setCursorPosition(std::nullopt);
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
        case internal_types::MouseKeyEvent::LeftButtonReleased:
            XTestFakeButtonEvent(display, leftButton, False, CurrentTime);
            break;
        case internal_types::MouseKeyEvent::MiddleButtonPressed:
            XTestFakeButtonEvent(display, middleButton, True, CurrentTime);
            break;
        case internal_types::MouseKeyEvent::MiddleButtonReleased:
            XTestFakeButtonEvent(display, middleButton, False, CurrentTime);
            break;
        case internal_types::MouseKeyEvent::RightButtonPressed:
            XTestFakeButtonEvent(display, rightButton, True, CurrentTime);
            break;
        case internal_types::MouseKeyEvent::RightButtonReleased:
            XTestFakeButtonEvent(display, rightButton, False, CurrentTime);
            break;
    }
    XFlush(display);
}

void MouseReceiver::onEvent(const internal_types::MouseChangePositionEvent& event)
{
    setCursorPosition(event);
}

void MouseReceiver::setCursorPosition(const std::optional<internal_types::MouseChangePositionEvent>& event)
{
    if (event)
    {
        dispatchState = DispatchState::on;
    }
    else
    {
        dispatchState = DispatchState::off;
    }
    cursorGuard->setPosition(event);
}

void MouseReceiver::setContactPoints(
    const std::pair<internal_types::Point, internal_types::Point>& contactPoints,
    const internal_types::Point& diffPointForSend,
    const internal_types::Point& diffPointForReceive)
{
    if (cursorGuard)
    {
        cursorGuard->setContactPoints(contactPoints, diffPointForSend, diffPointForReceive);
    }
    else
    {
        std::cerr << "MouseReceiver doesn't exist" << std::endl;
    }
}
} // namespace event_consumer
