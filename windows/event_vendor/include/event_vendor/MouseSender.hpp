#pragma once

#include <functional>
#include <optional>
#include "connection/Sender.hpp"
#include "internal_types/MouseEvent.hpp"

namespace commons
{
class CursorGuard;
}

namespace event_vendor
{
class MouseSender
{
public:
    MouseSender(std::shared_ptr<connection::Sender>, std::shared_ptr<commons::CursorGuard>);
    MouseSender() = delete;

    void start(std::function<void()>&&);
    void changeMouseState(const std::optional<internal_types::MouseChangePositionEvent>&);

    LRESULT forwardEvent(int nCode, WPARAM, LPARAM);

private:
    LRESULT sendEvent(internal_types::MouseEvent&&);

    std::shared_ptr<connection::Sender> sender;
    bool isEventSending;
    std::shared_ptr<commons::CursorGuard> cursorGuard;

    HHOOK mouseHook;
    std::function<void()> changeKeyboardState;
};
} // namespace event_vendor
