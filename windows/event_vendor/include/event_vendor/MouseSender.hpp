#pragma once

#include <functional>
#include <optional>
#include "connection/Sender.hpp"
#include "internal_types/MouseEvent.hpp"

namespace event_vendor
{
class MouseSender
{
public:
    MouseSender(std::shared_ptr<connection::Sender>);
    MouseSender() = delete;

    void start(std::function<void()>&&);
    void changeMouseState(const std::optional<internal_types::MouseChangePositionEvent>&);

private:
    LRESULT dispatchWindowsEvent(int, WPARAM, LPARAM);

    std::shared_ptr<connection::Sender> sender;
    bool isEventSending;

    HHOOK mouseHook;
};
} // namespace event_vendor
