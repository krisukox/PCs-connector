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

App::App(int argc, char* argv[]) : socket{std::make_unique<connection::Socket>()}
{
    auto port = std::string("10000");

    auto callback = [this]() {
        sender = std::make_shared<connection::Sender>(socket->value());

        auto stopAppCallback = [this] { socket->close(); };

        auto keyboard = std::make_shared<event_vendor::KeyboardSender>(sender);
        auto mouse = std::make_shared<event_vendor::MouseSender>(sender);
        auto receiver =
            std::make_shared<connection::Receiver>(socket->value(), std::make_unique<internal_types::Deserializer>());

        vendor = std::make_shared<app_management::Vendor>(
            keyboard, mouse, receiver, socket->getIoContext(), std::move(stopAppCallback));
        vendor->start();
    };

    try
    {
        if (argc == 1)
        {
            auto address = boost::asio::ip::make_address("192.168.1.5");
            socket->connect(address, port, callback);
        }
        else
        {
            auto address = boost::asio::ip::make_address(argv[1]);
            socket->connect(address, port, callback);
        }
    }
    catch (...)
    {
    }
}
} // namespace app_management
