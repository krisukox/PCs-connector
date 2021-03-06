#include "event_vendor/KeyboardSender.hpp"
#include <cassert>
#include <iostream>

namespace
{
constexpr uint8_t VK_A = 0x41;
constexpr uint8_t VK_Q = 0x51;
constexpr uint8_t VK_L = 0x4C;

constexpr LRESULT ignoreEvent = 1;

namespace keyboard_callback
{
std::function<LRESULT(internal_types::KeyEvent)> dispatchEvent = nullptr;

LRESULT resultCallback(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (!dispatchEvent)
    {
        throw std::runtime_error("dispatchEvent pointer not defined");
    }

    if (nCode == HC_ACTION)
    {
        DWORD keyCode = reinterpret_cast<PKBDLLHOOKSTRUCT>(lParam)->vkCode;

        switch (wParam)
        {
            case WM_KEYDOWN:
            case WM_SYSKEYDOWN:
            {
                return dispatchEvent(internal_types::KeyEvent{std::uint8_t(keyCode), true});
            }
            case WM_KEYUP:
            case WM_SYSKEYUP:
            {
                return dispatchEvent(internal_types::KeyEvent{std::uint8_t(keyCode), false});
            }
        }
    }
    return 1;
};
} // namespace keyboard_callback
} // namespace

namespace event_vendor
{
KeyboardSender::KeyboardSender() : isEventSending{false} {}

void KeyboardSender::start(ForwardEvent&& _eventReceived)
{
    forwardEvent = std::move(_eventReceived);

    start();
}

void KeyboardSender::start()
{
    keyboard_callback::dispatchEvent = [this](const internal_types::KeyEvent& keyEvent) -> LRESULT {
        changeKeyMod(keyEvent);

        if (checkForLockComputer(keyEvent))
        {
            lockComputer();
        }
        else if (isEventSending)
        {
            if (checkForIgnoreCtrl(keyEvent))
            {
                return ignoreEvent;
            }
            else if (checkForRAltPress(keyEvent))
            {
                forwardEvent(internal_types::KeyEvent{VK_LCONTROL, false});
            }
            forwardEvent(keyEvent);
        }

        return isEventSending;
    };

    keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, keyboard_callback::resultCallback, nullptr, NULL);
}

bool KeyboardSender::checkForIgnoreCtrl(const internal_types::KeyEvent& keyEvent)
{
    return (isRAltPressed && keyEvent.keyCode == VK_LCONTROL);
}

void KeyboardSender::changeKeyMod(const internal_types::KeyEvent& keyEvent)
{
    if (keyEvent.keyCode == VK_LCONTROL)
    {
        isCtrlPressed = keyEvent.isPressed;
    }
    if (keyEvent.keyCode == VK_LSHIFT)
    {
        isShiftPressed = keyEvent.isPressed;
    }
    if (keyEvent.keyCode == VK_LWIN)
    {
        isLWinPressed = keyEvent.isPressed;
    }
    if (keyEvent.keyCode == VK_RMENU)
    {
        isRAltPressed = keyEvent.isPressed;
    }
}

void KeyboardSender::changeState()
{
    if (isEventSending)
    {
        isEventSending = false;
    }
    else
    {
        isEventSending = true;
    }
}

bool KeyboardSender::checkForRAltPress(const internal_types::KeyEvent& keyEvent)
{
    return (isCtrlPressed && keyEvent.keyCode == VK_RMENU && keyEvent.isPressed);
}

bool KeyboardSender::checkForLockComputer(const internal_types::KeyEvent& keyEvent)
{
    return (isLWinPressed && keyEvent.keyCode == VK_L && keyEvent.isPressed);
}

void KeyboardSender::lockComputer()
{
    forwardEvent(internal_types::KeyEvent{VK_LWIN, true});
    forwardEvent(internal_types::KeyEvent{VK_L, true});
    forwardEvent(internal_types::KeyEvent{VK_L, false});
    forwardEvent(internal_types::KeyEvent{VK_LWIN, false});
}
} // namespace event_vendor
