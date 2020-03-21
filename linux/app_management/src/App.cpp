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

void App::listen(int argc, char* argv[], commons::IApp::SetScreenResolution setScreenResolution)
{
    std::cout << " LISTEN1 " << pthread_self() << std::endl;

    auto successfullConnection = [this, argc, argv, setScreenResolution](boost::asio::ip::tcp::socket& socket) {
        std::cout << " LISTEN2 " << pthread_self() << std::endl;
        auto receiver =
            std::make_shared<connection::Receiver>(socket, std::make_unique<internal_types::Deserializer>(display));

        auto successfullCallback = [setScreenResolution](const internal_types::ScreenResolution& screenResolution) {
            std::cout << " LISTEN3 " << pthread_self() << std::endl;
            std::cout << "ScreenResolution received" << screenResolution.width << " " << screenResolution.height
                      << std::endl;
            if (setScreenResolution == nullptr)
            {
                std::cout << "NULL PTR" << std::endl;
            }
            if (setScreenResolution) setScreenResolution(screenResolution);
        };
        auto unsuccessfullCallback = [](const boost::system::error_code&) {};
        std::cout << " LISTEN4 " << pthread_self() << std::endl;
        receiver->synchronizedReceive<internal_types::ScreenResolution>(
            std::move(successfullCallback), std::move(unsuccessfullCallback));
        std::cout << "FKUNAWD!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
        //        std::make_shared<Consumer>(
        //            keyboardReceiverSelector(argc, argv),
        //            std::make_shared<event_consumer::MouseReceiver>(
        //                display, std::make_unique<connection::Sender>(socket), cursorGuard),
        //            std::move(receiver))
        //            ->start();
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
