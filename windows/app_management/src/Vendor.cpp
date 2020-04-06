#include "app_management/Vendor.hpp"
#include <iostream>
#include "connection/Receiver.hpp"
#include "connection/Socket.hpp"
#include "event_vendor/KeyboardSender.hpp"
#include "event_vendor/MouseSender.hpp"

namespace app_management
{
Vendor::Vendor(
    std::unique_ptr<event_vendor::KeyboardSender> keyboardSender,
    event_vendor::MouseSender* mouseSender,
    std::unique_ptr<connection::Socket> _socket,
    std::function<void(internal_types::ScreenResolution)> _setScreenResolution)
    : keyboard{std::move(keyboardSender)}
    , mouse{mouseSender}
    , socket{std::move(_socket)}
    , setScreenResolution{_setScreenResolution}
{
    socket->start();
}

Vendor::~Vendor()
{
    PostThreadMessage(eventCatchingThreadId, WM_QUIT, 0, 0);
    eventCatchingThread.join();
}

void Vendor::start(const internal_types::ScreenResolution& masterScreenResolution)
{
    connection::Receiver::SuccessfulCallback<internal_types::ScreenResolution> successfulCallback =
        [this](internal_types::ScreenResolution screenResolution) {
            setScreenResolution(screenResolution);
            registerForMouseChangePositionEvent();
            eventCatchingThread = std::thread(&Vendor::startCatchingEvents, this);
        };

    socket->receiveOnce(successfulCallback);
    socket->send(masterScreenResolution);
}

void Vendor::setContactPoints(
    const std::pair<internal_types::Point, internal_types::Point>& contactPoints,
    const internal_types::Point& diffPointForSend,
    const internal_types::Point& diffPointForReceive)
{
    if (mouse)
    {
        mouse->setContactPoints(contactPoints, diffPointForSend, diffPointForReceive);
    }
    else
    {
        std::cerr << "Mouse doesn't exist" << std::endl;
    }
}

void Vendor::registerForMouseChangePositionEvent()
{
    connection::Receiver::SuccessfulCallback<internal_types::Event> successfulCallback =
        [this](const internal_types::Event& event) {
            keyboard->changeState();
            mouse->changeMouseState(
                std::get<internal_types::MouseChangePositionEvent>(std::get<internal_types::MouseEvent>(event)));
        };
    socket->receive(successfulCallback);
}

void Vendor::startCatchingEvents()
{
    eventCatchingThreadId = GetCurrentThreadId();
    keyboard->start([this](const internal_types::KeyEvent& keyEvent) { socket->send(keyEvent); });
    mouse->start(std::bind(&Vendor::changeKeyboardState, this), [this](const internal_types::MouseEvent& mouseEvent) {
        socket->send(mouseEvent);
    });

    MSG msg;
    BOOL retVal;
    while ((retVal = GetMessage(&msg, nullptr, 0, 0)) != 0)
    {
        if (retVal == -1)
        {
            //            stopAppCallback();
        }
    }
}

void Vendor::changeKeyboardState()
{
    keyboard->changeState();
}
} // namespace app_management
