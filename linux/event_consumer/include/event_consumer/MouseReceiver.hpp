#pragma once

#include <X11/Xlib.h>
#include "IMouseReceiver.hpp"
#include "internal_types/Point.hpp"

namespace event_consumer
{
class MouseReceiver : public IMouseReceiver
{
public:
    MouseReceiver(Display*);
    ~MouseReceiver() override;

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
} // namespace event_consumer
