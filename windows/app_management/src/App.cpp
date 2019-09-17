#include "app_management/App.hpp"
#include "app_management/Vendor.hpp"
#include "connection/Sender.hpp"
#include "connection/Socket.hpp"
#include "event_vendor/KeyboardSender.hpp"
#include "event_vendor/MouseSender.hpp"

namespace app_management
{
App::~App() {}

App::App(int argc, char* argv[]) : socket{std::make_unique<connection::Socket>()}
{
    auto port = std::string("10000");
    try
    {
        if (argc == 1)
        {
            auto address = boost::asio::ip::make_address("192.168.1.5");
            socket->connect(address, port);
        }
        else
        {
            auto address = boost::asio::ip::make_address(argv[1]);
            socket->connect(address, port);
        }
    }
    catch (...)
    {
    }

    sender = std::make_shared<connection::Sender>(socket->value());

    auto stopAppCallback = [this] { socket->close(); };

    auto keyboard = std::make_unique<event_vendor::KeyboardSender>(sender);
    auto mouse = std::make_unique<event_vendor::MouseSender>(sender);

    vendor = std::make_unique<app_management::Vendor>(std::move(keyboard), std::move(mouse));
    vendor->start(std::move(stopAppCallback));
}
} // namespace app_management
