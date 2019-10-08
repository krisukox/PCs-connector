#include "app_management/Vendor.hpp"
#include <iostream>
#include <thread>
#include "connection/IReceiver.hpp"
#include "event_vendor/KeyboardSender.hpp"
#include "event_vendor/MouseSender.hpp"

namespace app_management
{
Vendor::Vendor(
    std::shared_ptr<event_vendor::KeyboardSender> keyboardSender,
    std::shared_ptr<event_vendor::MouseSender> mouseSender,
    std::shared_ptr<connection::IReceiver> receiver_,
    boost::asio::io_context& ioContext_,
    std::function<void()>&& stopAppCallback_)
    : keyboard{keyboardSender}
    , mouse{mouseSender}
    , receiver{receiver_}
    , ioContext{ioContext_}
    , stopAppCallback{std::move(stopAppCallback_)}
{
}

void Vendor::start()
{
    std::thread t(&Vendor::startCatchingEvents, this);

    read();
    ioContext.run();
    t.join();
}

void Vendor::read()
{
    receiver->receive(
        [this](const internal_types::Event& event) {
            keyboard->changeState();
            mouse->changeMouseState(
                std::get<internal_types::MouseChangePositionEvent>(std::get<internal_types::MouseEvent>(event)));
            read();
        },
        [](boost::system::error_code) { std::cerr << "Unsuccessful event receive" << std::endl; });
}

void Vendor::startCatchingEvents()
{
    auto stopApp = [this] {
        PostMessage(nullptr, WM_QUIT, 0, 0);
        ioContext.stop();
        stopAppCallback();
    };

    keyboard->start(std::move(stopApp));
    mouse->start([this]() { keyboard->changeState(); });

    MSG msg;
    BOOL retVal;
    while ((retVal = GetMessage(&msg, nullptr, 0, 0)) != 0)
    {
        if (retVal == -1)
        {
            ioContext.stop();
            stopAppCallback();
        }
    }
}
} // namespace app_management
