#include "app_management/App.hpp"
#include <functional>
#include <iostream>
#include "app_management/Vendor.hpp"
#include "connection/Receiver.hpp"
#include "connection/Sender.hpp"
#include "connection/Socket.hpp"
#include "event_vendor/KeyboardSender.hpp"
#include "event_vendor/MouseSender.hpp"
#include "internal_types/DeserializerWin.hpp"

namespace app_management
{
App::~App() {}

App::App() : socket{std::make_unique<connection::Socket>()} {}

void App::start(int argc, char* argv[])
{
    auto port = std::string("10000");

    try
    {
        if (argc == 1)
        {
            auto address = boost::asio::ip::make_address("192.168.1.10");
            socket->connect(address, port);

            initializeVendor();
            std::thread t(&Vendor::startCatchingEvents, vendor);
            vendor->startReceivingEvents();
            socket->getIoContext().run();
            t.join();
        }
        else
        {
            auto address = boost::asio::ip::make_address(argv[1]);
            socket->connect(address, port);

            initializeVendor();
            std::thread t(&Vendor::startCatchingEvents, vendor);
            vendor->startReceivingEvents();
            socket->getIoContext().run();
            t.join();
        }
    }
    catch (std::exception e)
    {
        std::clog << e.what() << std::endl;
    }
}

void App::initializeVendor()
{
    auto stopAppCallback = [this] { socket->close(); };

    auto receiver =
        std::make_shared<connection::Receiver>(socket->value(), std::make_unique<internal_types::Deserializer>());
    auto sender = std::make_shared<connection::Sender>(socket->value());

    auto keyboard = std::make_shared<event_vendor::KeyboardSender>(sender);
    auto mouse = std::make_shared<event_vendor::MouseSender>(sender);

    vendor = std::make_shared<app_management::Vendor>(keyboard, mouse, receiver, stopAppCallback);
}
} // namespace app_management
