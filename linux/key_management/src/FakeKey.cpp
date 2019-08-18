#include "key_management/FakeKey.hpp"
#include <X11/extensions/XTest.h>
#include <X11/keysym.h>
#include <iostream>
#include <memory>
#include <unordered_map>

// XTestFakeButtonEvent(display, 3, True, CurrentTime); //Mouse down
// XTestFakeButtonEvent(display, 3, False, CurrentTime); //Mouse up

namespace key_management
{
FakeKey::FakeKey() : display{XOpenDisplay(nullptr)}, deserializer{display} {}

FakeKey::~FakeKey()
{
    XCloseDisplay(display);
}

void FakeKey::handleEvent(std::byte keyId, bool isPressed) const
{
    try
    {
        KeyCode keycode = deserializer.decode(keyId);
        XTestFakeKeyEvent(display, keycode, isPressed, CurrentTime);
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
}
} // namespace key_management
