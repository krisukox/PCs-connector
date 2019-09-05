#pragma once

#include <X11/Xlib.h>
#include "IMouse.hpp"

namespace mouse_management
{
class FakeMouse : public IMouse
{
public:
    FakeMouse(Display*);
    ~FakeMouse() override;

    void onEvent(const internal_types::MouseEvent&) const override;

private:
    void onEvent(const internal_types::MouseMoveEvent&) const;
    void onEvent(const internal_types::MouseScrollEvent&) const;
    void onEvent(const internal_types::MouseKeyEvent&) const;

    Display* display;
};
} // namespace mouse_management
