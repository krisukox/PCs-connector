#pragma once

#include <functional>
#include <memory>
#include "connection/Sender.hpp"
#include "internal_types/KeyEvent.hpp"

namespace event_vendor
{
class KeyboardSender
{
public:
    using EventReceived = std::function<void(const internal_types::KeyEvent&)>;

    KeyboardSender();

    void start(EventReceived);
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

    EventReceived eventReceived;
    HHOOK keyboardHook;
};
} // namespace event_vendor
