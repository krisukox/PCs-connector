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
    KeyboardSender(std::shared_ptr<connection::Sender>);
    KeyboardSender() = delete;

    void start(std::function<void()>&&);
    void changeState();

private:
    void start();
    bool checkForStopApp(const internal_types::KeyEvent&);
    void stopApp();
    bool checkForIgnoreCtrl(const internal_types::KeyEvent&);
    void changeKeyMod(const internal_types::KeyEvent&);
    bool checkForRAltPress(const internal_types::KeyEvent&);
    bool checkForLockComputer(const internal_types::KeyEvent&);

    void lockComputer();

    bool isCtrlPressed = false;
    bool isShiftPressed = false;
    bool isLWinPressed = false;
    bool isRAltPressed = false;

    std::shared_ptr<connection::Sender> sender;
    std::function<void()> stopAppCallback;
    bool isEventSending;

    HHOOK keyboardHook;
};
} // namespace event_vendor
