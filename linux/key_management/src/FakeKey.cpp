#include "key_management/FakeKey.hpp"
#include <X11/extensions/XTest.h>
#include <X11/keysym.h>
#include <iostream>

// XTestFakeButtonEvent(display, 3, True, CurrentTime); //Mouse down
// XTestFakeButtonEvent(display, 3, False, CurrentTime); //Mouse up

namespace key_management
{
FakeKey::FakeKey()
{
    display = XOpenDisplay(nullptr);
}

FakeKey::~FakeKey()
{
    XCloseDisplay(display);
}

void FakeKey::handleEvent(std::uint16_t keyId) const
{
    //        XK_KP_Enter
    KeyCode keycode = XKeysymToKeycode(display, keyId);
    XTestFakeKeyEvent(display, keycode, True, CurrentTime);
    XTestFakeKeyEvent(display, keycode, False, CurrentTime);
    XFlush(display);
}
} // namespace key_management
