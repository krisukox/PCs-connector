#pragma once

#include <X11/Xlib.h>
#include "IMouse.hpp"
#include "internal_types/Point.hpp"

namespace mouse_management
{
class FakeMouse : public IMouse
{
public:
    FakeMouse(Display*);
    ~FakeMouse() override;

    void onEvent(const internal_types::MouseEvent&) override;

private:
    void onEvent(const internal_types::MouseMoveEvent&);
    void onEvent(const internal_types::MouseScrollEvent&);
    void onEvent(const internal_types::MouseKeyEvent&);

    class CursorGuard
    {
    public:
        CursorGuard(Display*);
        bool checkIfCursorOutOfScreen(const internal_types::MouseMoveEvent&);

    private:
        internal_types::Point getMouseXCoordinate();

        Display* display;
        Window window;

        Window _w_;
        int _i_;
        unsigned _u_;
        int xCoordinate, yCoordinate;
    };

    Display* display;
    CursorGuard cursorGuard;
};
} // namespace mouse_management
