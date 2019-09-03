#include "key_management/FakeKey.hpp"
#include <X11/extensions/XTest.h>
#include <stdexcept>

namespace key_management
{
FakeKey::FakeKey(Display* display_) : display{display_} {}

FakeKey::~FakeKey()
{
    XCloseDisplay(display);
}

void FakeKey::onEvent(internal_types::KeyEvent keyEvent) const try
{
    XTestFakeKeyEvent(display, keyEvent.keyCode, keyEvent.isPressed, CurrentTime);
    XFlush(display);
}
catch (const std::out_of_range&)
{
    throw std::invalid_argument("Key not supported");
}
catch (...)
{
    throw std::runtime_error("Unexpected exception");
}
} // namespace key_management
