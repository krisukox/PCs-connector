#pragma once

#include <X11/Xlib.h>
#include <functional>
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
    using ForwardEvent = std::function<void(const internal_types::MouseChangePositionEvent&)>;

    MouseReceiver(Display*, std::unique_ptr<commons::CursorGuard>);
    ~MouseReceiver() override;

    void start(ForwardEvent) override;
    void onEvent(const internal_types::MouseEvent&) override;

    void setContactPoints(
        const std::pair<internal_types::Point, internal_types::Point>&,
        const internal_types::Point&,
        const internal_types::Point&) override;

private:
    void onEvent(const internal_types::MouseMoveEvent&);
    void onEvent(const internal_types::MouseScrollEvent&);
    void onEvent(const internal_types::MouseKeyEvent&);
    void onEvent(const internal_types::MouseChangePositionEvent&);

    void setCursorPosition(const std::optional<internal_types::MouseChangePositionEvent>&);

    Display* display;
    Window window;
    std::unique_ptr<commons::CursorGuard> cursorGuard;

    DispatchState dispatchState;

    ForwardEvent forwardEvent;
};
} // namespace event_consumer
