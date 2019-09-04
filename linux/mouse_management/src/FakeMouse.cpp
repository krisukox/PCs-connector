#include "mouse_management/FakeMouse.hpp"
#include <X11/extensions/XTest.h>
#include <stdexcept>

namespace mouse_management
{
FakeMouse::FakeMouse(Display* display_) : display{display_} {}

FakeMouse::~FakeMouse()
{
    XCloseDisplay(display);
}

void FakeMouse::onEvent(internal_types::MouseMoveEvent mouseMoveEvent) const
{
    XTestFakeRelativeMotionEvent(display, mouseMoveEvent.deltaX, mouseMoveEvent.deltaY, CurrentTime);
    XFlush(display);
}
} // namespace mouse_management
