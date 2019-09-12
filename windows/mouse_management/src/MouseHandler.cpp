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
} // namespace mouse_callback
} // namespace

Mouse::Mouse(
    std::function<void(internal_types::MouseEvent)>&& _pressedKeyCallback,
    std::function<void()>&& _stopAppCallback)
    : pressedKeyCallback{std::move(_pressedKeyCallback)}, stopAppCallback{std::move(_stopAppCallback)}
{
}

void Mouse::start()
{
    mouse_callback::pressedKeyCallbackGlob = [this](internal_types::MouseEvent event) -> LRESULT {
        //        std::byte byteX1{static_cast<uint8_t>(mouseEvent.deltaX >> 8)};
        //        std::byte byteX2{static_cast<uint8_t>(mouseEvent.deltaX)};                SERIALIZER
        //        std::byte byteY1{static_cast<uint8_t>(mouseEvent.deltaY >> 8)};
        //        std::byte byteY2{static_cast<uint8_t>(mouseEvent.deltaY)};

        pressedKeyCallback(event);

        return 1;
    };

    auto callback = [](int nCode, WPARAM wParam, LPARAM lParam) -> LRESULT {
        if (nCode == HC_ACTION)
        {
            auto mouseStruct = reinterpret_cast<PMSLLHOOKSTRUCT>(lParam);

            if (wParam == WM_MOUSEMOVE)
            {
                POINT point = mouseStruct->pt;

                return mouse_callback::pressedKeyCallbackGlob(
                    internal_types::MouseMoveEvent{static_cast<short>(point.x), static_cast<short>(point.y)});

                //                return mouse_callback::pressedKeyCallbackGlob(mouse_callback::MouseEvent{
                //                    static_cast<unsigned short>(point.x), static_cast<unsigned short>(point.y)});
            }
        }
        return 1;
    };

    POINT cursorPosition;
    GetCursorPos(&cursorPosition);
    SetCursorPos(0, 0);

    assert(mouse_callback::pressedKeyCallbackGlob);
    mouseHook = SetWindowsHookEx(WH_MOUSE_LL, callback, nullptr, NULL);
}
