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
    std::unique_ptr<event_vendor::MouseSender> mouseSender,
    std::unique_ptr<connection::Socket> _socket,
    std::function<void(internal_types::ScreenResolution)> _setScreenResolution)
    : keyboard{std::move(keyboardSender)}
    , mouse{std::move(mouseSender)}
    , socket{std::move(_socket)}
    , setScreenResolution{_setScreenResolution}
{
}

Vendor::~Vendor()
{
    std::cout << "Vendor::~Vendor11" << std::endl;
    PostThreadMessage(eventCatchingThreadId, WM_QUIT, 0, 0);
    std::cout << "Vendor::~Vendor22" << std::endl;
    eventCatchingThread.join();
    std::cout << "Vendor::~Vendor33" << std::endl;
}

void Vendor::start(const internal_types::ScreenResolution& masterScreenResolution)
{
    connection::Receiver::SuccessfulCallback<internal_types::ScreenResolution> successfulCallback =
        [this](internal_types::ScreenResolution screenResolution) { setScreenResolution(screenResolution); };

    socket->send(masterScreenResolution);
    socket->receiveOnce(successfulCallback);
    registerForMouseChangePositionEvent();
    eventCatchingThread = std::thread(&Vendor::startCatchingEvents, this);
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
    connection::Receiver::SuccessfulCallback<internal_types::DecodedType> successfullCallback =
        [this](const internal_types::DecodedType& decoded) {
            std::visit(
                internal_types::Visitor{
                    [this](const internal_types::Event& event) { handleReceivedEvent(event); },
                    [](const internal_types::ScreenResolution&) {},
                },
                decoded);
        };

    socket->receive(std::move(successfullCallback));
}

void Vendor::handleReceivedEvent(const internal_types::Event& event)
{
    keyboard->changeState();
    mouse->changeMouseState(
        std::get<internal_types::MouseChangePositionEvent>(std::get<internal_types::MouseEvent>(event)));
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
            // stop
        }
    }
}

void Vendor::changeKeyboardState()
{
    keyboard->changeState();
}
} // namespace app_management
