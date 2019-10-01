#include "app_management/App.hpp"
#include <iostream>
#include "app_management/Consumer.hpp"
#include "connection/Receiver.hpp"
#include "connection/Socket.hpp"
#include "event_consumer/KeyboardReceiver.hpp"
#include "event_consumer/MouseReceiver.hpp"
#include "event_consumer/TestKeyboardReceiver.hpp"
#include "internal_types/Deserializer.hpp"

namespace app_management
{
App::App(int argc, char* argv[]) : display{XOpenDisplay(nullptr)}, socket{std::make_unique<connection::Socket>()}
{
    auto successfullConnection = [this, argc, argv](boost::asio::ip::tcp::socket& socket) {
        consumer = std::make_unique<Consumer>(
            keyboardReceiverSelector(argc, argv),
            std::make_shared<event_consumer::MouseReceiver>(display),
            std::make_shared<connection::Receiver>(socket, std::make_unique<internal_types::Deserializer>(display)));
        consumer->start();
    };

    socket->listen("10000", successfullConnection);
}

App::~App()
{
    XCloseDisplay(display);
}

std::shared_ptr<event_consumer::IKeyboardReceiver> App::keyboardReceiverSelector(int argc, char* argv[])
{
    if (argc == 2 && !std::strcmp(argv[1], "test"))
    {
        std::clog << "test mode started" << std::endl;
        return std::make_shared<event_consumer::TestKeyboardReceiver>();
    }
    else if (argc == 1)
    {
        std::clog << "release mode started" << std::endl;
        return std::make_shared<event_consumer::KeyboardReceiver>(display);
    }
    throw std::invalid_argument("not valid startup argument");
}

} // namespace app_management
