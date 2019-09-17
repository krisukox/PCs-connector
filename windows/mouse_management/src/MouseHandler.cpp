#include "mouse_management/MouseHandler.hpp"
#include <Windows.h>
#include <cassert>
#include <iostream>
#include <windowsx.h>
#include "internal_types/MouseEvent.hpp"

namespace
{
namespace mouse_callback
{
std::function<LRESULT(internal_types::MouseEvent)> pressedKeyCallbackGlob = nullptr;
std::function<LRESULT()> changeStateCallback = nullptr;
std::function<LRESULT()> returnDefault = nullptr;
} // namespace mouse_callback
} // namespace

Mouse::Mouse(
    std::function<void(internal_types::MouseEvent)>&& _pressedKeyCallback,
    std::function<void()>&& _stopAppCallback,
    std::function<void()>&& _changeKeyboardState)
    : pressedKeyCallback{std::move(_pressedKeyCallback)}
    , stopAppCallback{std::move(_stopAppCallback)}
    , changeKeyboardState{std::move(_changeKeyboardState)}
{
}

void Mouse::start()
{
    mouse_callback::pressedKeyCallbackGlob = [this](internal_types::MouseEvent event) -> LRESULT {
        if (!hookState)
        {
            return 0;
        }
        pressedKeyCallback(event);
        return 1;
    };

    mouse_callback::changeStateCallback = [this]() -> LRESULT {
        changeState();
        changeKeyboardState();
        return hookState;
    };

    mouse_callback::returnDefault = [this]() -> LRESULT { return hookState; };

    auto callback = [](int nCode, WPARAM wParam, LPARAM lParam) -> LRESULT {
        if (nCode == HC_ACTION)
        {
            auto mouseStruct = reinterpret_cast<PMSLLHOOKSTRUCT>(lParam);

            switch (wParam)
            {
                case WM_MOUSEMOVE:
                {
                    POINT point = mouseStruct->pt;

                    if (point.x == 0 && !mouse_callback::returnDefault())
                    {
                        return mouse_callback::changeStateCallback();
                    }

                    return mouse_callback::pressedKeyCallbackGlob(
                        internal_types::MouseMoveEvent{static_cast<short>(point.x), static_cast<short>(point.y)});
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
                    return mouse_callback::returnDefault();
            }
        }
        return 1;
    };

    POINT cursorPosition;
    GetCursorPos(&cursorPosition);
    //    SetCursorPos(0, 0);

    assert(mouse_callback::returnDefault);
    assert(mouse_callback::pressedKeyCallbackGlob);
    mouseHook = SetWindowsHookEx(WH_MOUSE_LL, callback, nullptr, NULL);
}

void Mouse::changeState()
{
    if (hookState)
    {
        hookState = false;
    }
    else
    {
        hookState = true;
        SetCursorPos(0, 0);
    }
}
