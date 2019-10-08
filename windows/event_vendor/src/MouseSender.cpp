#include "event_vendor/MouseSender.hpp"
#include <cassert>
#include <iostream>
#include <optional>
#include <stdexcept>

namespace
{
namespace mouse_callback
{
std::function<LRESULT()> isEventSending = nullptr;
std::function<LRESULT(internal_types::MouseEvent&&, POINT&)> dispatchEvent = nullptr;

LRESULT resultCallback(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (!isEventSending || !dispatchEvent)
    {
        throw std::runtime_error("isEventSending or dispatchEvent pointer not defined");
    }
    if (nCode == HC_ACTION)
    {
        auto mouseStruct = reinterpret_cast<PMSLLHOOKSTRUCT>(lParam);

        switch (wParam)
        {
            case WM_MOUSEMOVE:
            {
                POINT point = mouseStruct->pt;
                return mouse_callback::dispatchEvent(
                    internal_types::MouseMoveEvent{static_cast<short>(point.x), static_cast<short>(point.y)}, point);
            }
            case WM_LBUTTONDOWN:
            case WM_LBUTTONUP:
            case WM_LBUTTONDBLCLK:
            case WM_RBUTTONDOWN:
            case WM_RBUTTONUP:
            case WM_RBUTTONDBLCLK:
            case WM_MBUTTONDOWN:
            case WM_MBUTTONUP:
            case WM_MBUTTONDBLCLK:
            case WM_MOUSEWHEEL:
                return mouse_callback::isEventSending();
        }
    }
    return 1;
};
} // namespace mouse_callback
} // namespace

namespace event_vendor
{
MouseSender::MouseSender(std::shared_ptr<connection::Sender> _sender) : sender{_sender}, isEventSending{false} {}

void MouseSender::start(std::function<void()>&& changeKeyboardState)
{
    mouse_callback::dispatchEvent = [this, changeKeyboardState = std::move(changeKeyboardState)](
                                        internal_types::MouseEvent&& mouseEvent,
                                        const std::optional<POINT>& point) -> LRESULT {
        if (isEventSending)
        {
            sender->send(mouseEvent);
        }
        if (!isEventSending && point && point->x <= -1)
        {
            changeMouseState(std::nullopt);
            changeKeyboardState();
            sender->send(
                internal_types::MouseChangePositionEvent{static_cast<short>(point->x), static_cast<short>(point->y)});
        }
        return isEventSending;
    };

    mouse_callback::isEventSending = [this]() -> LRESULT { return isEventSending; };

    mouseHook = SetWindowsHookEx(WH_MOUSE_LL, mouse_callback::resultCallback, nullptr, NULL);
}

void MouseSender::changeMouseState(const std::optional<internal_types::MouseChangePositionEvent>& mouseEvent)
{
    if (mouseEvent)
    {
        isEventSending = false;
        SetCursorPos(mouseEvent->x, mouseEvent->y);
    }
    else
    {
        isEventSending = true;
        SetCursorPos(0, 0);
    }
}
} // namespace event_vendor
