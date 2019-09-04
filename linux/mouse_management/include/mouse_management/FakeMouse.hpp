#pragma once

#include <X11/Xlib.h>
#include <cstdint>
#include "internal_types/MouseEvent.hpp"

namespace mouse_management
{
class FakeMouse
{
public:
    FakeMouse(Display*);
    ~FakeMouse();
    void onEvent(internal_types::MouseMoveEvent) const;

private:
    Display* display;
};
} // namespace mouse_management
