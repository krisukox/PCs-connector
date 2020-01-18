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
    auto successfullCallback = [self = shared_from_this()](const internal_types::Event& event) {
        std::visit(
            internal_types::Visitor{
                [self](const internal_types::KeyEvent& keyEvent) { self->keyReceiver->onEvent(keyEvent); },
                [self](const internal_types::MouseEvent& mouseEvent) { self->mouseReceiver->onEvent(mouseEvent); },
            },
            event);
        self->readBody();
    };

    auto unsuccessfullCallback = [self = shared_from_this()](const boost::system::error_code& errorCode) {
        if (!errorCode)
        {
            self->readBody();
        }
    };

    receiver->receive(std::move(successfullCallback), std::move(unsuccessfullCallback));
}
} // namespace app_management
