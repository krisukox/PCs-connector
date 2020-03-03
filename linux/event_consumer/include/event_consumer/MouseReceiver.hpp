#pragma once

#include <X11/Xlib.h>
#include <memory>
#include <optional>
#include "CursorGuard.hpp"
#include "IMouseReceiver.hpp"
#include "internal_types/Point.hpp"

namespace connection
{
class Sender;
}

namespace event_consumer
{
class CursorGuard;

enum class DispatchState
{
    on,
    off
};

class MouseReceiver : public IMouseReceiver
{
public:
    MouseReceiver(Display*, std::unique_ptr<connection::Sender>, std::shared_ptr<CursorGuard>);
    ~MouseReceiver() override;

    void onEvent(const internal_types::MouseEvent&) override;

private:
    void onEvent(const internal_types::MouseMoveEvent&);
    void onEvent(const internal_types::MouseScrollEvent&);
    void onEvent(const internal_types::MouseKeyEvent&);
    void onEvent(const internal_types::MouseChangePositionEvent&);

    void setCursorPosition(const internal_types::MouseChangePositionEvent&);

    internal_types::MouseChangePositionEvent changeToRelative(const internal_types::MouseChangePositionEvent& event);

    Display* display;
    std::unique_ptr<connection::Sender> sender;
    std::shared_ptr<CursorGuard> cursorGuard;

    DispatchState dispatchState;
    Screen* screen;
};
} // namespace event_consumer
