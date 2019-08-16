#include "Keyboard.hpp"
//#include <Windows.h>
#include <cassert>
#include <iostream>

namespace
{
namespace keyboard_callback
{
std::function<void(asciiCode)> pressedKeyCallbackGlob = nullptr;
} // namespace keyboard_callback
} // namespace

Keyboard::Keyboard(std::function<void(asciiCode)>&& _pressedKeyCallback, std::function<void()>&& _stopAppCallback)
    : pressedKeyCallback{std::move(_pressedKeyCallback)}, stopAppCallback{std::move(_stopAppCallback)}
{
    keyboard_callback::pressedKeyCallbackGlob = pressedKeyCallback;
    auto callback = [](int nCode, WPARAM wParam, LPARAM lParam) -> LRESULT {
        if (nCode == HC_ACTION)
        {
            DWORD keyCode = reinterpret_cast<PKBDLLHOOKSTRUCT>(lParam)->vkCode;

            switch (wParam)
            {
                case WM_KEYDOWN:
                case WM_SYSKEYDOWN:
                {
                    keyboard_callback::pressedKeyCallbackGlob(
                        asciiCode{std::byte(std::uint8_t(keyCode)), std::byte(1)});
                }
                break;
                case WM_KEYUP:
                case WM_SYSKEYUP:
                {
                    keyboard_callback::pressedKeyCallbackGlob(
                        asciiCode{std::byte(std::uint8_t(keyCode)), std::byte(0)});
                }
            }
        }
        return 1;
    };

    assert(keyboard_callback::pressedKeyCallbackGlob);
    keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)callback, nullptr, NULL);

    MSG msg;
    GetMessage(&msg, nullptr, 0, 0);
}
