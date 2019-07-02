#include "../include/FakeKey.hpp"
#include <X11/extensions/XTest.h>
#include <X11/keysym.h>

// XTestFakeButtonEvent(display, 3, True, CurrentTime); //Mouse down
// XTestFakeButtonEvent(display, 3, False, CurrentTime); //Mouse up

FakeKey::FakeKey()
{
    display = XOpenDisplay(NULL);
}

FakeKey::~FakeKey()
{
    XCloseDisplay(display);
}

void FakeKey::pressKey(u_int16_t keyId)
{
    //        XK_KP_Enter
    KeyCode keycode = XKeysymToKeycode(display, keyId);
    XTestFakeKeyEvent(display, keycode, True, CurrentTime);
    XTestFakeKeyEvent(display, keycode, False, CurrentTime);
    XFlush(display);
}
