#include "app_management/Vendor.hpp"
#include <iostream>
#include "connection/Receiver.hpp"
#include "event_vendor/KeyboardSender.hpp"
#include "event_vendor/MouseSender.hpp"

namespace app_management
{
Vendor::Vendor(
    std::shared_ptr<event_vendor::KeyboardSender> keyboardSender,
    std::shared_ptr<event_vendor::MouseSender> mouseSender,
    std::shared_ptr<connection::Receiver> receiver_,
    std::function<void()> stopAppCallback_)
    : keyboard{keyboardSender}, mouse{mouseSender}, receiver{receiver_}, stopAppCallback{stopAppCallback_}
{
}

void Vendor::startReceivingEvents()
{
    receiveEvent();
}

void Vendor::receiveEvent()
{
    receiver->receive<internal_types::Event>(
        [this](internal_types::Event event) {
            keyboard->changeState();
            mouse->changeMouseState(
                std::get<internal_types::MouseChangePositionEvent>(std::get<internal_types::MouseEvent>(event)));
            startReceivingEvents();
        },
        [this](boost::system::error_code) {
            std::cerr << "Unsuccessful event receive" << std::endl;
            stopApp();
        });
}

void Vendor::startCatchingEvents()
{
    keyboard->start(std::bind(&Vendor::stopApp, this));
    mouse->start(std::bind(&Vendor::changeKeyboardState, this));

    MSG msg;
    BOOL retVal;
    while ((retVal = GetMessage(&msg, nullptr, 0, 0)) != 0)
    {
        if (retVal == -1)
        {
            stopAppCallback();
        }
    }
}

void Vendor::stopApp()
{
    PostMessage(nullptr, WM_QUIT, 0, 0);
    stopAppCallback();
}

void Vendor::changeKeyboardState()
{
    keyboard->changeState();
}
} // namespace app_management
