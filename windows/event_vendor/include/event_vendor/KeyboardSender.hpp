#pragma once

#include <Windows.h>
#include <functional>
#include <memory>
#include "internal_types/KeyEvent.hpp"

namespace event_vendor
{
class KeyboardSender
{
public:
    using ForwardEvent = std::function<void(const internal_types::KeyEvent&)>;

    KeyboardSender();

    void start(ForwardEvent&&);
    void changeState();

private:
    void start();

    bool checkForIgnoreCtrl(const internal_types::KeyEvent&);
    void changeKeyMod(const internal_types::KeyEvent&);
    bool checkForRAltPress(const internal_types::KeyEvent&);
    bool checkForLockComputer(const internal_types::KeyEvent&);

    void lockComputer();

    bool isCtrlPressed = false;
    bool isShiftPressed = false;
    bool isLWinPressed = false;
    bool isRAltPressed = false;

    bool isEventSending;

    ForwardEvent forwardEvent;
    HHOOK keyboardHook;
};
} // namespace event_vendor
