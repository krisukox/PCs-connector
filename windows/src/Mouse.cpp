#include "Mouse.hpp"
#include <Windows.h>
#include <cassert>
#include <iostream>
#include <windowsx.h>

namespace
{
namespace keyboard_callback
{
struct MouseEvent
{
    unsigned short deltaX;
    unsigned short deltaY;
};

std::function<LRESULT(MouseEvent)> pressedKeyCallbackGlob = nullptr;
} // namespace keyboard_callback
} // namespace

Mouse::Mouse(std::function<void(Buffer)>&& _pressedKeyCallback, std::function<void()>&& _stopAppCallback)
    : pressedKeyCallback{std::move(_pressedKeyCallback)}, stopAppCallback{std::move(_stopAppCallback)}
{
}

void Mouse::start()
{
    keyboard_callback::pressedKeyCallbackGlob = [this](keyboard_callback::MouseEvent mouseEvent) -> LRESULT {
        std::byte byteX1{static_cast<uint8_t>(mouseEvent.deltaX >> 8)};
        std::byte byteX2{static_cast<uint8_t>(mouseEvent.deltaX)};
        std::byte byteY1{static_cast<uint8_t>(mouseEvent.deltaY >> 8)};
        std::byte byteY2{static_cast<uint8_t>(mouseEvent.deltaY)};

        pressedKeyCallback(Buffer{std::byte(0b11111101), byteX1, byteX2, byteY1, byteY2});

        return 1;
    };

    auto callback = [](int nCode, WPARAM wParam, LPARAM lParam) -> LRESULT {
        if (nCode == HC_ACTION)
        {
            auto mouseStruct = reinterpret_cast<PMSLLHOOKSTRUCT>(lParam);

            if (wParam == WM_MOUSEMOVE)
            {
                POINT point = mouseStruct->pt;

                return keyboard_callback::pressedKeyCallbackGlob(keyboard_callback::MouseEvent{
                    static_cast<unsigned short>(point.x), static_cast<unsigned short>(point.y)});
            }
        }
        return 1;
    };

    POINT cursorPosition;
    GetCursorPos(&cursorPosition);
    SetCursorPos(0, 0);

    assert(keyboard_callback::pressedKeyCallbackGlob);
    mouseHook = SetWindowsHookEx(WH_MOUSE_LL, callback, nullptr, NULL);
}
