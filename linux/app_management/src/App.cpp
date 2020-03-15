#include "app_management/App.hpp"
#include <iostream>
#include "app_management/Consumer.hpp"
#include "connection/Receiver.hpp"
#include "connection/Sender.hpp"
#include "connection/Socket.hpp"
#include "event_consumer/CursorGuard.hpp"
#include "event_consumer/KeyboardReceiver.hpp"
#include "event_consumer/MouseReceiver.hpp"
#include "event_consumer/TestKeyboardReceiver.hpp"
#include "internal_types/Deserializer.hpp"

namespace app_management
{
App::App()
    : display{XOpenDisplay(nullptr)}
    , socket{std::make_unique<connection::Socket>()}
    , cursorGuard{std::make_shared<event_consumer::CursorGuard>(
          XWidthOfScreen(XDefaultScreenOfDisplay(display)),
          XHeightOfScreen(XDefaultScreenOfDisplay(display)))}
{
}

App::~App()
{
    XCloseDisplay(display);
}

void App::start(int argc, char* argv[])
{
    auto successfullConnection = [this, argc, argv](boost::asio::ip::tcp::socket& socket) {
        std::make_shared<Consumer>(
            keyboardReceiverSelector(argc, argv),
            std::make_shared<event_consumer::MouseReceiver>(
                display, std::make_unique<connection::Sender>(socket), cursorGuard),
            std::make_shared<connection::Receiver>(socket, std::make_unique<internal_types::Deserializer>(display)))
            ->start();
    };

    socket->listen("10000", successfullConnection);
}

void App::setContactPoints(
    const std::pair<internal_types::Point, internal_types::Point>& contactPoints,
    const internal_types::Point& diffPoint)
{
    cursorGuard->setContactPoints(contactPoints, diffPoint);
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
