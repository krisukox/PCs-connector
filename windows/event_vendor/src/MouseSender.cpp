#include "event_vendor/MouseSender.hpp"
#include <cassert>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <utility>
#include "commons/CursorGuard.hpp"
#include "internal_types/Point.hpp"

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

constexpr std::pair<internal_types::Point, internal_types::Point> contactPoints = {{0, 700}, {0, 1080}};
constexpr internal_types::Point diffPoint = {1366, -700};

bool isCursorInsideScreen(const internal_types::Point& cursor)
{
    return cursor.x < 1920 && cursor.x >= 0 && cursor.y < 1080 && cursor.y >= 0;
}

bool isCursorOutOfContactArea(const internal_types::Point& cursor)
{
    if (contactPoints.first.x == contactPoints.second.x)
    {
        if (contactPoints.first.x == 0)
        {
            if (cursor.x > 0)
            {
                return true;
            }
        }
        else if (cursor.x < 0)
        {
            return true;
        }
        if (contactPoints.first.y < contactPoints.second.y)
        {
            return cursor.y < contactPoints.first.y || cursor.y > contactPoints.second.y;
        }
        return cursor.y > contactPoints.first.y || cursor.y < contactPoints.second.y;
    }
    if (contactPoints.first.y == 0)
    {
        if (cursor.y > 0)
        {
            return true;
        }
    }
    else if (cursor.y < 0)
    {
        return true;
    }
    if (contactPoints.first.x < contactPoints.second.x)
    {
        return cursor.x < contactPoints.first.x || cursor.x > contactPoints.second.x;
    }
    return cursor.x > contactPoints.first.x || cursor.x < contactPoints.second.x;
}
} // namespace

MouseSender::MouseSender(
    std::shared_ptr<connection::Sender> _sender,
    std::shared_ptr<commons::CursorGuard> _cursorGuard)
    : sender{_sender}, isEventSending{false}, cursorGuard{_cursorGuard}
{
}

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
        POINT cursor = mouseStruct->pt;
        if (!isEventSending && wParam == WM_MOUSEMOVE)
        {
            auto mouseChangePositionEvent =
                cursorGuard->checkIfCursorOutOfScreen({static_cast<short>(cursor.x), static_cast<short>(cursor.y)});
            if (mouseChangePositionEvent)
            {
                changeMouseState(std::nullopt);
                changeKeyboardState();
                return sendEvent(mouseChangePositionEvent.value());
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
                    return sendEvent(internal_types::MouseKeyEvent::LeftButtonReleased);
                case WM_MBUTTONDOWN:
                    return sendEvent(internal_types::MouseKeyEvent::MiddleButtonPressed);
                case WM_MBUTTONUP:
                    return sendEvent(internal_types::MouseKeyEvent::MiddleButtonReleased);
                case WM_RBUTTONDOWN:
                    return sendEvent(internal_types::MouseKeyEvent::RightButtonPressed);
                case WM_RBUTTONUP:
                    return sendEvent(internal_types::MouseKeyEvent::RightButtonReleased);
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
