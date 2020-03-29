#pragma once

#include <X11/Xlib.h>
#include <memory>
#include <optional>
#include "IMouseReceiver.hpp"
#include "internal_types/Point.hpp"

namespace connection
{
class Sender;
}

namespace commons
{
class CursorGuard;
}

namespace event_consumer
{
enum class DispatchState
{
    on,
    off
};

class MouseReceiver : public IMouseReceiver
{
public:
    MouseReceiver(Display*, std::shared_ptr<connection::Sender>, std::shared_ptr<commons::CursorGuard>);
    ~MouseReceiver() override;

    void onEvent(const internal_types::MouseEvent&) override;

private:
    void onEvent(const internal_types::MouseMoveEvent&);
    void onEvent(const internal_types::MouseScrollEvent&);
    void onEvent(const internal_types::MouseKeyEvent&);
    void onEvent(const internal_types::MouseChangePositionEvent&);

    void setCursorPosition(const std::optional<internal_types::MouseChangePositionEvent>&);

    Display* display;
    Window window;
    std::shared_ptr<connection::Sender> sender;
    std::shared_ptr<commons::CursorGuard> cursorGuard;

    DispatchState dispatchState;
};
} // namespace event_consumer
