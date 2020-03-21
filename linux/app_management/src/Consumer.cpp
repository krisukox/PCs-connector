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
    std::shared_ptr<connection::Receiver> _receiver)
    : keyReceiver{std::move(_keyReceiver)}, mouseReceiver{std::move(_mouseReceiver)}, receiver{_receiver}
{
}

void Consumer::start()
{
    //    std::this_thread::sleep_for(std::chrono::seconds(2));
    unsigned long long index = 0;
    while (true)
    {
        index++;
        if (index == 5000000000)
        {
            break;
        }
        if (index % 1000000000 == 0)
        {
            //            index = 0;
            std::cout << " !START " << pthread_self() << std::endl;
        }
    }
    //    readBody();
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

    receiver->receive<internal_types::Event>(std::move(successfullCallback), std::move(unsuccessfullCallback));
}
} // namespace app_management
