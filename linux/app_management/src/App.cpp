#include "app_management/App.hpp"
#include <sys/types.h>
#include <iostream>
#include <unistd.h>
#include "app_management/Consumer.hpp"
#include "connection/Receiver.hpp"
#include "connection/Sender.hpp"
#include "connection/Socket.hpp"
#include "event_consumer/KeyboardReceiver.hpp"
#include "event_consumer/MouseReceiver.hpp"
#include "event_consumer/TestKeyboardReceiver.hpp"
#include "internal_types/Deserializer.hpp"
#include "internal_types/ScreenResolution.hpp"

namespace app_management
{
App::App(std::shared_ptr<commons::CursorGuard>&& _cursorGuard)
    : commons::IApp(std::move(_cursorGuard))
    , display{XOpenDisplay(nullptr)}
    , socket{std::make_unique<connection::Socket>()}
{
    cursorGuard = std::make_shared<commons::CursorGuard>(
        XWidthOfScreen(XDefaultScreenOfDisplay(display)), XHeightOfScreen(XDefaultScreenOfDisplay(display)));
}

App::~App()
{
    XCloseDisplay(display);
}

void App::listen(
    int argc,
    char* argv[],
    commons::IApp::SetScreenResolution setScreenResolution,
    const internal_types::ScreenResolution& masterScreenResolution)
{
    auto successfullConnection =
        [this, argc, argv, setScreenResolution, masterScreenResolution](boost::asio::ip::tcp::socket& socket) {
            auto receiver =
                std::make_shared<connection::Receiver>(socket, std::make_unique<internal_types::Deserializer>(display));

            auto sender = std::make_shared<connection::Sender>(socket);

            auto successfullCallback = [setScreenResolution, masterScreenResolution, sender](
                                           const internal_types::ScreenResolution& slaveScreenResolution) {
                setScreenResolution(slaveScreenResolution);
                sender->send(masterScreenResolution);
            };
            auto unsuccessfullCallback = [](const boost::system::error_code&) {};
            receiver->synchronizedReceive<internal_types::ScreenResolution>(
                std::move(successfullCallback), std::move(unsuccessfullCallback));
            std::make_shared<Consumer>(
                keyboardReceiverSelector(argc, argv),
                std::make_shared<event_consumer::MouseReceiver>(display, sender, cursorGuard),
                std::move(receiver))
                ->start();
        };

    socket->listen("10000", successfullConnection);
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
