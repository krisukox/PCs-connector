#include "mouse_management/FakeMouse.hpp"
#include <X11/extensions/XTest.h>
#include <stdexcept>

namespace mouse_management
{
FakeMouse::FakeMouse(Display* _display) : display{_display}, cursorGuard{display} {}

FakeMouse::~FakeMouse() = default;

void FakeMouse::onEvent(const internal_types::MouseEvent& mouseEvent)
{
    std::visit([this](const auto& event) { onEvent(event); }, mouseEvent);
}

void FakeMouse::onEvent(const internal_types::MouseMoveEvent& mouseMoveEvent)
{
    if (cursorGuard.checkIfCursorOutOfScreen(mouseMoveEvent))
    {
        // OUT OF SCREEN
    }
    XTestFakeRelativeMotionEvent(display, mouseMoveEvent.deltaX, mouseMoveEvent.deltaY, CurrentTime);
    XFlush(display);
}

void FakeMouse::onEvent(const internal_types::MouseScrollEvent&) {}

void FakeMouse::onEvent(const internal_types::MouseKeyEvent&) {}

FakeMouse::CursorGuard::CursorGuard(Display* _display) : display{_display}, window{XRootWindow(display, 0)} {}

internal_types::Point FakeMouse::CursorGuard::getMouseXCoordinate()
{
    if (!XQueryPointer(display, window, &_w_, &_w_, &xCoordinate, &yCoordinate, &_i_, &_i_, &_u_))
    {
        throw std::runtime_error("wrong XQueryPointer result");
    }
    return {static_cast<short>(xCoordinate), static_cast<short>(yCoordinate)};
}

bool FakeMouse::CursorGuard::checkIfCursorOutOfScreen(const internal_types::MouseMoveEvent& mouseMoveEvent)
{
    internal_types::Point newPoint = getMouseXCoordinate() + mouseMoveEvent;
    return newPoint.x >= XWidthOfScreen(XDefaultScreenOfDisplay(display));
}
} // namespace mouse_management
