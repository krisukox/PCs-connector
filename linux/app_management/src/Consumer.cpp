#include "app_management/Consumer.hpp"
#include <iostream>
#include <stdexcept>
#include "connection/Receiver.hpp"
#include "connection/Socket.hpp"
#include "event_consumer/KeyboardReceiver.hpp"
#include "event_consumer/MouseReceiver.hpp"
#include "internal_types/MouseEvent.hpp"
#include "internal_types/Visitor.hpp"

namespace app_management
{
Consumer::Consumer(
    std::unique_ptr<event_consumer::IKeyboardReceiver> _keyReceiver,
    std::unique_ptr<event_consumer::IMouseReceiver> _mouseReceiver,
    std::unique_ptr<connection::Socket> _socket,
    std::function<void(internal_types::ScreenResolution)> _setScreenResolution)
    : keyReceiver{std::move(_keyReceiver)}
    , mouseReceiver{std::move(_mouseReceiver)}
    , socket{std::move(_socket)}
    , setScreenResolution{_setScreenResolution}
{
    socket->start();
    mouseReceiver->start([this](const internal_types::MouseChangePositionEvent& event) { socket->send(event); });
}

void Consumer::start(const internal_types::ScreenResolution& masterScreenResolution)
{
    connection::Receiver::SuccessfulCallback<internal_types::ScreenResolution> successfulCallback =
        [this, masterScreenResolution](internal_types::ScreenResolution screenResolution) {
            std::cout << "SCREEN RESOLUTION CALLBACK 1" << std::endl;
            setScreenResolution(screenResolution);
            registerForEvent();
            socket->send(masterScreenResolution);
        };
    socket->receiveOnce(successfulCallback);
}

void Consumer::registerForEvent()
{
    connection::Receiver::SuccessfulCallback<internal_types::Event> successfullCallback =
        [this](const internal_types::Event& event) {
            std::visit(
                internal_types::Visitor{
                    [this](const internal_types::KeyEvent& keyEvent) { keyReceiver->onEvent(keyEvent); },
                    [this](const internal_types::MouseEvent& mouseEvent) { mouseReceiver->onEvent(mouseEvent); },
                },
                event);
        };

    socket->receive(std::move(successfullCallback));
}

void Consumer::setContactPoints(
    const std::pair<internal_types::Point, internal_types::Point>& contactPoints,
    const internal_types::Point& diffPointForSend,
    const internal_types::Point& diffPointForReceive)
{
    if (mouseReceiver)
    {
        mouseReceiver->setContactPoints(contactPoints, diffPointForSend, diffPointForReceive);
    }
    else
    {
        std::cerr << "MouseReceiver doesn't exist" << std::endl;
    }
}
} // namespace app_management
