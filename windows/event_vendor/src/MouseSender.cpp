#include "event_vendor/MouseSender.hpp"
#include <cassert>
#include <iostream>
#include <optional>
#include <stdexcept>

namespace event_vendor
{
namespace
{
event_vendor::MouseSender* This;

LRESULT resultCallback(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (!This)
    {
        std::cerr << "'This' pointer not defined\n";
        PostMessage(nullptr, WM_QUIT, 0, 0);
    }
    return This->forwardEvent(nCode, wParam, lParam);
};

constexpr LRESULT eventSent = 1;
constexpr LRESULT eventHeld = 0;
} // namespace

MouseSender::MouseSender(std::shared_ptr<connection::Sender> _sender) : sender{_sender}, isEventSending{false} {}

void MouseSender::start(std::function<void()>&& _changeKeyboardState)
{
    This = this;
    changeKeyboardState = std::move(_changeKeyboardState);

    mouseHook = SetWindowsHookEx(WH_MOUSE_LL, resultCallback, nullptr, NULL);
}

LRESULT MouseSender::forwardEvent(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == HC_ACTION)
    {
        auto mouseStruct = reinterpret_cast<PMSLLHOOKSTRUCT>(lParam);
        if (!isEventSending && wParam == WM_MOUSEMOVE)
        {
            POINT point = mouseStruct->pt;
            if (point.x <= -1)
            {
                changeMouseState(std::nullopt);
                changeKeyboardState();
                return sendEvent(
                    internal_types::MouseChangePositionEvent{static_cast<short>(point.x), static_cast<short>(point.y)});
            }
            return eventHeld;
        }
        else if (isEventSending)
        {
            switch (wParam)
            {
                case WM_MOUSEMOVE:
                {
                    POINT point = mouseStruct->pt;
                    return sendEvent(
                        internal_types::MouseMoveEvent{static_cast<short>(point.x), static_cast<short>(point.y)});
                }
                case WM_LBUTTONDOWN:
                    return sendEvent(internal_types::MouseKeyEvent::LeftButtonPressed);
                case WM_LBUTTONUP:
                    return sendEvent(internal_types::MouseKeyEvent::LeftButtonUnpressed);
                case WM_MBUTTONDOWN:
                    return sendEvent(internal_types::MouseKeyEvent::MiddleButtonPressed);
                case WM_MBUTTONUP:
                    return sendEvent(internal_types::MouseKeyEvent::MiddleButtonUnpressed);
                case WM_RBUTTONDOWN:
                    return sendEvent(internal_types::MouseKeyEvent::RightButtonPressed);
                case WM_RBUTTONUP:
                    return sendEvent(internal_types::MouseKeyEvent::RightButtonUnpressed);
                case WM_MOUSEWHEEL:
                    if (static_cast<std::make_signed_t<WORD>>(HIWORD(mouseStruct->mouseData)) < 0)
                    {
                        return sendEvent(internal_types::MouseScrollEvent::Backward);
                    }
                    else
                    {
                        return sendEvent(internal_types::MouseScrollEvent::Forward);
                    }
                case WM_LBUTTONDBLCLK:
                case WM_RBUTTONDBLCLK:
                case WM_MBUTTONDBLCLK:
                    return eventSent;
            }
        }
        else
        {
            return eventHeld;
        }
    }
    return 1;
}

LRESULT MouseSender::sendEvent(internal_types::MouseEvent&& mouseEvent)
{
    sender->send(mouseEvent);
    return eventSent;
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
