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

void FakeKey::handleEvent(std::byte keyId, std::byte isPressed) const
{
    try
    {
        if (bool(isPressed & std::byte(0b01000000)))
        {
            std::cerr << "MOUSE" << std::endl;
            if (bool(isPressed & std::byte(0b00001000)))
            {
                XTestFakeRelativeMotionEvent(display, 0, -1, CurrentTime);
            }
            else if (bool(isPressed & std::byte(0b00000100)))
            {
                XTestFakeRelativeMotionEvent(display, 0, 1, CurrentTime);
            }
            else if (bool(isPressed & std::byte(0b00000010)))
            {
                XTestFakeRelativeMotionEvent(display, -1, 0, CurrentTime);
            }
            else if (bool(isPressed & std::byte(0b00000001)))
            {
                XTestFakeRelativeMotionEvent(display, 1, 0, CurrentTime);
            }
            XFlush(display);
        }
        else
        {
            KeyCode keycode = deserializer.decode(keyId);
            XTestFakeKeyEvent(display, keycode, bool(isPressed), CurrentTime);
            //            XTestFakeRelativeMotionEvent(display, 1, 1, CurrentTime);
            XFlush(display);
        }
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
