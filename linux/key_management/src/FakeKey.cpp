#include "key_management/FakeKey.hpp"
#include <X11/extensions/XTest.h>
#include <stdexcept>

namespace key_management
{
FakeKey::FakeKey(Display* display_) : display{display_} {}

FakeKey::~FakeKey() = default;

void FakeKey::onEvent(const internal_types::KeyEvent& keyEvent) const
{
    XTestFakeKeyEvent(display, keyEvent.keyCode, keyEvent.isPressed, CurrentTime);
    XFlush(display);
}
} // namespace key_management
