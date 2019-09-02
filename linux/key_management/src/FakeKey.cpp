#include "key_management/FakeKey.hpp"
#include <X11/extensions/XTest.h>
#include <X11/keysym.h>
#include <iostream>
#include <memory>
#include <unordered_map>
#include "internal_types/KeyEvent.hpp"

// XTestFakeButtonEvent(display, 3, True, CurrentTime); //Mouse down
// XTestFakeButtonEvent(display, 3, False, CurrentTime); //Mouse up

namespace key_management
{
FakeKey::FakeKey(Display* display_) : display{display_}, deserializer{display} {}

FakeKey::~FakeKey()
{
    XCloseDisplay(display);
}

void FakeKey::onEvent(internal_types::KeyEvent keyEvent) const try
{
    std::clog << ">>>  " << static_cast<uint16_t>(keyEvent.keyCode) << std::endl;
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
