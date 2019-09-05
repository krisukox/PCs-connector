#include "mouse_management/FakeMouse.hpp"
#include <X11/extensions/XTest.h>

namespace mouse_management
{
FakeMouse::FakeMouse(Display* display_) : display{display_} {}

FakeMouse::~FakeMouse() = default;

void FakeMouse::onEvent(const internal_types::MouseEvent& mouseEvent) const
{
    std::visit([this](const auto& event) { onEvent(event); }, mouseEvent);
}

void FakeMouse::onEvent(const internal_types::MouseMoveEvent& mouseMoveEvent) const
{
    XTestFakeRelativeMotionEvent(display, mouseMoveEvent.deltaX, mouseMoveEvent.deltaY, CurrentTime);
    XFlush(display);
}

void FakeMouse::onEvent(const internal_types::MouseScrollEvent&) const {}

void FakeMouse::onEvent(const internal_types::MouseKeyEvent&) const {}
} // namespace mouse_management
