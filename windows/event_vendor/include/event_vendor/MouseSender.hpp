#pragma once

#include <functional>
#include <optional>
#include "commons/CursorGuard.hpp"
#include "connection/Sender.hpp"
#include "internal_types/MouseEvent.hpp"
#include "internal_types/Point.hpp"

namespace commons
{
class CursorGuard;
}

namespace event_vendor
{
class MouseSender
{
public:
    using EventReceived = std::function<void(const internal_types::MouseEvent&)>;

    MouseSender(std::unique_ptr<commons::CursorGuard>);

    void start(std::function<void()>&&, EventReceived);
    void changeMouseState(const std::optional<internal_types::MouseChangePositionEvent>&);

    LRESULT forwardEvent(int nCode, WPARAM, LPARAM);

    void setContactPoints(
        const std::pair<internal_types::Point, internal_types::Point>&,
        const internal_types::Point&,
        const internal_types::Point&);

private:
    void start();
    LRESULT sendEvent(internal_types::MouseEvent&&);

    bool isEventSending;
    std::unique_ptr<commons::CursorGuard> cursorGuard;

    std::function<void()> changeKeyboardState;
    EventReceived eventReceived;
    HHOOK mouseHook;
};
} // namespace event_vendor
