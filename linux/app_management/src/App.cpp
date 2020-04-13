#include "app_management/App.hpp"
#include <iostream>
#include "Deserializer.hpp"
#include "app_management/Consumer.hpp"
#include "connection/Socket.hpp"
#include "event_consumer/KeyboardReceiver.hpp"
#include "event_consumer/MouseReceiver.hpp"
#include "event_consumer/TestKeyboardReceiver.hpp"
#include "internal_types/ScreenResolution.hpp"

namespace app_management
{
App::App() : display{XOpenDisplay(nullptr)} {}

App::~App()
{
    XCloseDisplay(display);
}

void App::listen(
    int argc,
    char* argv[],
    const internal_types::ScreenResolution& masterScreenResolution,
    internal_types::SetScreenResolution&& setScreenResolution)
{
    auto keyboardReceiver = selectKeyboardReceiver(argc, argv);
    auto mouseReceiver =
        std::make_unique<event_consumer::MouseReceiver>(display, std::make_unique<commons::CursorGuard>());

    auto port = std::string("10000");
    auto socket = std::make_unique<connection::Socket>(port, std::make_unique<internal_types::Deserializer>(display));

    consumer = std::make_unique<Consumer>(
        std::move(keyboardReceiver), std::move(mouseReceiver), std::move(socket), std::move(setScreenResolution));
    consumer->start(masterScreenResolution);
}

std::unique_ptr<event_consumer::IKeyboardReceiver> App::selectKeyboardReceiver(int argc, char* argv[])
{
    if (argc == 2 && !std::strcmp(argv[1], "test"))
    {
        std::clog << "test mode started" << std::endl;
        return std::make_unique<event_consumer::TestKeyboardReceiver>();
    }
    else if (argc == 1)
    {
        std::clog << "release mode started" << std::endl;
        return std::make_unique<event_consumer::KeyboardReceiver>(display);
    }
    throw std::invalid_argument("not valid startup argument");
}

void App::setContactPoints(
    const std::pair<internal_types::Point, internal_types::Point>& contactPoints,
    const internal_types::Point& diffPointForSend,
    const internal_types::Point& diffPointForReceive)
{
    if (consumer)
    {
        consumer->setContactPoints(contactPoints, diffPointForSend, diffPointForReceive);
    }
    else
    {
        std::cerr << "Consumer doesn't exist" << std::endl;
    }
}
} // namespace app_management
