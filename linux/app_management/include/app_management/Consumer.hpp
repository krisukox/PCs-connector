#pragma once

#include <functional>
#include <iostream>
#include <memory>
#include "connection/Socket.hpp"
#include "event_consumer/KeyboardReceiver.hpp"
#include "event_consumer/MouseReceiver.hpp"
#include "internal_types/CommonTypes.hpp"
#include "internal_types/MouseEvent.hpp"
#include "internal_types/Point.hpp"
#include "internal_types/ScreenResolution.hpp"
#include "internal_types/Visitor.hpp"

namespace app_management
{
template <class SocketType>
class Consumer
{
public:
    Consumer(
        std::unique_ptr<event_consumer::IKeyboardReceiver> _keyReceiver,
        std::unique_ptr<event_consumer::IMouseReceiver> _mouseReceiver,
        std::unique_ptr<SocketType> _socket,
        internal_types::SetScreenResolution _setScreenResolution)
        : keyReceiver{std::move(_keyReceiver)}
        , mouseReceiver{std::move(_mouseReceiver)}
        , socket{std::move(_socket)}
        , setScreenResolution{std::move(_setScreenResolution)}
    {
        mouseReceiver->start([this](const internal_types::MouseChangePositionEvent& event) { socket->send(event); });
    }

    void start(const internal_types::ScreenResolution& masterScreenResolution)
    {
        connection::Socket::SuccessfulCallback<internal_types::ScreenResolution> successfulCallback =
            [this, masterScreenResolution](internal_types::ScreenResolution screenResolution) {
                setScreenResolution(screenResolution);
                startReceiving();
                socket->send(masterScreenResolution);
            };
        socket->receiveOnce(std::move(successfulCallback));
    }

    void setContactPoints(
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

private:
    void startReceiving()
    {
        connection::Socket::SuccessfulCallback<internal_types::DecodedType> successfullCallback =
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

    void handleReceivedEvent(const internal_types::Event& event)
    {
        std::visit(
            internal_types::Visitor{
                [this](const internal_types::KeyEvent& keyEvent) { keyReceiver->onEvent(keyEvent); },
                [this](const internal_types::MouseEvent& mouseEvent) { mouseReceiver->onEvent(mouseEvent); },
            },
            event);
    }

    std::unique_ptr<event_consumer::IKeyboardReceiver> keyReceiver;
    std::unique_ptr<event_consumer::IMouseReceiver> mouseReceiver;
    std::unique_ptr<SocketType> socket;
    internal_types::SetScreenResolution setScreenResolution;
};
} // namespace app_management
