#include "app_management/Consumer.hpp"
#include <iostream>
#include <stdexcept>
#include "connection/Receiver.hpp"
#include "event_consumer/KeyboardReceiver.hpp"
#include "event_consumer/MouseReceiver.hpp"
#include "internal_types/Visitor.hpp"

namespace app_management
{
Consumer::Consumer(
    std::shared_ptr<event_consumer::IKeyboardReceiver> _keyReceiver,
    std::shared_ptr<event_consumer::IMouseReceiver> _mouseReceiver,
    std::shared_ptr<connection::IReceiver> _receiver)
    : keyReceiver{std::move(_keyReceiver)}, mouseReceiver{std::move(_mouseReceiver)}, receiver{_receiver}
{
}

void Consumer::start()
{
    readBody();
}

void Consumer::readBody()
{
    auto successfullCallback = [this](const internal_types::Event& event) {
        std::visit(
            internal_types::Visitor{
                [this](const internal_types::KeyEvent& keyEvent) { keyReceiver->onEvent(keyEvent); },
                [this](const internal_types::MouseEvent& mouseEvent) { mouseReceiver->onEvent(mouseEvent); },
            },
            event);
        readBody();
    };

    auto unsuccessfullCallback = [this](const boost::system::error_code& errorCode) {
        if (!errorCode)
        {
            readBody();
        }
        else
        {
            // LEAVE
        }
    };

    receiver->receive(std::move(successfullCallback), std::move(unsuccessfullCallback));
}
} // namespace app_management
