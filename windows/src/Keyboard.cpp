#include "Keyboard.hpp"
#include <cassert>
#include <iostream>

namespace
{
constexpr uint8_t WIN_A = 0x41;
constexpr uint8_t WIN_Q = 0x51;
constexpr uint8_t WIN_L = 0x4C;

namespace keyboard_callback
{
std::function<LRESULT(internal_types::KeyEvent)> pressedKeyCallbackGlob = nullptr;
} // namespace keyboard_callback
} // namespace

Keyboard::Keyboard(
    std::function<void(internal_types::Event)>&& _pressedKeyCallback,
    std::function<void()>&& _stopAppCallback)
    : pressedKeyCallback{std::move(_pressedKeyCallback)}, stopAppCallback{std::move(_stopAppCallback)}
{
}

void Keyboard::start()
{
    keyboard_callback::pressedKeyCallbackGlob = [this](internal_types::KeyEvent keyEvent) -> LRESULT {
        changeCtrl(keyEvent);
        changeShift(keyEvent);
        if (isCtrlPressed && isShiftPressed && keyEvent.keyCode == WIN_A && keyEvent.isPressed)
        {
            changeState();
            return 1;
        }
        if (isCtrlPressed && isShiftPressed && keyEvent.keyCode == WIN_Q && keyEvent.isPressed)
        {
            PostMessage(nullptr, WM_QUIT, 0, 0);
            return 1;
        }
        if (!hookState)
        {
            return 0;
        }
        if (keyEvent.keyCode == WIN_L && keyEvent.isPressed)
        {
            pressedKeyCallback(keyEvent);
            pressedKeyCallback(internal_types::KeyEvent{VK_LWIN, false});
            pressedKeyCallback(internal_types::KeyEvent{WIN_L, false});
        }
        else
        {
            pressedKeyCallback(keyEvent);
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
                        internal_types::KeyEvent{std::uint8_t(keyCode), true});
                }
                case WM_KEYUP:
                case WM_SYSKEYUP:
                {
                    return keyboard_callback::pressedKeyCallbackGlob(
                        internal_types::KeyEvent{std::uint8_t(keyCode), false});
                }
            }
        }
        return 1;
    };

    assert(keyboard_callback::pressedKeyCallbackGlob);
    keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, callback, nullptr, NULL);
}

void Keyboard::changeState()
{
    if (hookState)
    {
        pressedKeyCallback(internal_types::KeyEvent{VK_LCONTROL, false});
        pressedKeyCallback(internal_types::KeyEvent{VK_LSHIFT, false});
        hookState = false;
    }
    else
    {
        hookState = true;
    }
}

void Keyboard::changeCtrl(internal_types::KeyEvent keyEvent)
{
    if (keyEvent.keyCode == VK_LCONTROL)
    {
        if (keyEvent.isPressed)
        {
            isCtrlPressed = true;
        }
        else
        {
            isCtrlPressed = false;
        }
    }
}

void Keyboard::changeShift(internal_types::KeyEvent keyEvent)
{
    if (keyEvent.keyCode == VK_LSHIFT)
    {
        if (keyEvent.isPressed)
        {
            isShiftPressed = true;
        }
        else
        {
            isShiftPressed = false;
        }
    }
}
