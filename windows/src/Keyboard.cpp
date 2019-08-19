#include "Keyboard.hpp"
//#include <Windows.h>
#include <cassert>
#include <iostream>

namespace
{
namespace keyboard_callback
{
std::function<LRESULT(AsciiCode)> pressedKeyCallbackGlob = nullptr;
} // namespace keyboard_callback
} // namespace

Keyboard::Keyboard(std::function<void(AsciiCode)>&& _pressedKeyCallback, std::function<void()>&& _stopAppCallback)
    : pressedKeyCallback{std::move(_pressedKeyCallback)}, stopAppCallback{std::move(_stopAppCallback)}
{
}

void Keyboard::start()
{
    keyboard_callback::pressedKeyCallbackGlob = [this](AsciiCode asciiCode) -> LRESULT {
        changeCtrl(asciiCode);
        changeShift(asciiCode);
        if (isCtrlPressed && isShiftPressed && std::uint8_t(asciiCode[0]) == 65 && std::uint8_t(asciiCode[1]))
        {
            changeState();
            return 1;
        }
        if (!hookState)
        {
            return 0;
        }
        if (std::uint8_t(asciiCode[0]) == 0x4C && std::uint8_t(asciiCode[1]))
        {
            pressedKeyCallback(asciiCode);
            pressedKeyCallback(AsciiCode{std::byte(std::uint8_t(VK_LWIN)), std::byte(0)});
            pressedKeyCallback(AsciiCode{std::byte(std::uint8_t(76)), std::byte(0)});
        }
        else
        {
            pressedKeyCallback(asciiCode);
        }
        return 1;
    };

    auto callback = [](int nCode, WPARAM wParam, LPARAM lParam) -> LRESULT {
        if (nCode == HC_ACTION)
        {
            DWORD keyCode = reinterpret_cast<PKBDLLHOOKSTRUCT>(lParam)->vkCode;

            switch (wParam)
            {
                case WM_KEYDOWN:
                case WM_SYSKEYDOWN:
                {
                    return keyboard_callback::pressedKeyCallbackGlob(
                        AsciiCode{std::byte(std::uint8_t(keyCode)), std::byte(1)});
                }
                case WM_KEYUP:
                case WM_SYSKEYUP:
                {
                    return keyboard_callback::pressedKeyCallbackGlob(
                        AsciiCode{std::byte(std::uint8_t(keyCode)), std::byte(0)});
                }
            }
        }
        return 1;
    };

    assert(keyboard_callback::pressedKeyCallbackGlob);
    keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, callback, nullptr, NULL);

    MSG msg;
    BOOL retVal;
    while ((retVal = GetMessage(&msg, nullptr, 0, 0)) != 0)
    {
        if (retVal == -1)
        {
            // handle the error and possibly exit
        }
    }
}

void Keyboard::changeState()
{
    if (hookState)
    {
        pressedKeyCallback(AsciiCode{std::byte(std::uint8_t(VK_LCONTROL)), std::byte(0)});
        pressedKeyCallback(AsciiCode{std::byte(std::uint8_t(VK_LSHIFT)), std::byte(0)});
        hookState = false;
    }
    else
    {
        hookState = true;
    }
}

void Keyboard::changeCtrl(AsciiCode asciiCode)
{
    if (std::uint8_t(asciiCode[0]) == VK_LCONTROL)
    {
        if (std::uint8_t(asciiCode[1]))
        {
            isCtrlPressed = true;
        }
        else
        {
            isCtrlPressed = false;
        }
    }
}

void Keyboard::changeShift(AsciiCode asciiCode)
{
    if (std::uint8_t(asciiCode[0]) == VK_LSHIFT)
    {
        if (std::uint8_t(asciiCode[1]))
        {
            isShiftPressed = true;
        }
        else
        {
            isShiftPressed = false;
        }
    }
}
