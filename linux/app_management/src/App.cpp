#include "app_management/App.hpp"
#include <iostream>
#include "Deserializer.hpp"
#include "app_management/Consumer.hpp"
#include "connection/Socket.hpp"
#include "event_consumer/KeyboardReceiver.hpp"
#include "event_consumer/MouseReceiver.hpp"
#include "event_consumer/TestKeyboardReceiver.hpp"
#include "internal_types/ScreenResolution.hpp"

namespace
{
auto port = std::string("10000");
}

namespace app_management
{
App::App() : display{XOpenDisplay(nullptr)} {}

App::~App()
{
    XCloseDisplay(display);
}

void App::listen(
    const internal_types::ScreenResolution& masterScreenResolution,
    internal_types::SetScreenResolution&& setScreenResolution)
{
    std::thread consumerThread([this, masterScreenResolution, setScreenResolution = std::move(setScreenResolution)]() {
        auto keyboardReceiver = std::make_unique<event_consumer::KeyboardReceiver>(display);
        auto mouseReceiver =
            std::make_unique<event_consumer::MouseReceiver>(display, std::make_unique<commons::CursorGuard>());

        consumer = std::make_unique<Consumer<connection::Socket>>(
            std::move(keyboardReceiver), std::move(mouseReceiver), createSocket(), std::move(setScreenResolution));
        consumer->start(masterScreenResolution);
    });
    consumerThread.detach();
}

void App::test(const internal_types::ScreenResolution& masterScreenResolution)
{
    std::thread consumerThread([this, masterScreenResolution]() {
        internal_types::SetScreenResolution setScreenResolution = [](internal_types::ScreenResolution) {};

        auto keyboardReceiver = std::make_unique<event_consumer::TestKeyboardReceiver>();
        auto mouseReceiver =
            std::make_unique<event_consumer::MouseReceiver>(display, std::make_unique<commons::CursorGuard>());

        consumer = std::make_unique<Consumer<connection::Socket>>(
            std::move(keyboardReceiver), std::move(mouseReceiver), createSocket(), std::move(setScreenResolution));
        consumer->start(masterScreenResolution);
    });
    consumerThread.detach();
}

std::unique_ptr<connection::Socket> App::createSocket()
{
    return std::make_unique<connection::Socket>(port, std::make_unique<internal_types::Deserializer>(display));
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

void App::setTransformationPoints(const internal_types::TransformationPoints& transformationPoints)
{
    if (consumer)
    {
        consumer->setTransformationPoints(transformationPoints);
    }
    else
    {
        std::cerr << "Consumer doesn't exist" << std::endl;
    }
}
} // namespace app_management
