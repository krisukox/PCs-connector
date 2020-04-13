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
    //    socket->start();
    mouseReceiver->start([this](const internal_types::MouseChangePositionEvent& event) { socket->send(event); });
}

void Consumer::start(const internal_types::ScreenResolution& masterScreenResolution)
{
    connection::Receiver::SuccessfulCallback<internal_types::ScreenResolution> successfulCallback =
        [this](internal_types::ScreenResolution screenResolution) { setScreenResolution(screenResolution); };
    socket->receiveOnce(successfulCallback);
    startReceiving();
    socket->send(masterScreenResolution);
}

void Consumer::startReceiving()
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

void Consumer::handleReceivedEvent(const internal_types::Event& event)
{
    std::visit(
        internal_types::Visitor{
            [this](const internal_types::KeyEvent& keyEvent) { keyReceiver->onEvent(keyEvent); },
            [this](const internal_types::MouseEvent& mouseEvent) { mouseReceiver->onEvent(mouseEvent); },
        },
        event);
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
