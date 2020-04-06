#include "app_management/App.hpp"
#include <functional>
#include <iostream>
#include "Deserializer.hpp"
#include "app_management/Vendor.hpp"
#include "connection/Sender.hpp"
#include "connection/Socket.hpp"
#include "event_vendor/KeyboardSender.hpp"
#include "event_vendor/MouseSender.hpp"
#include "internal_types/ScreenResolution.hpp"

namespace app_management
{
App::~App() = default;

App::App() {}

void App::connect(
    const boost::asio::ip::address& address,
    const internal_types::ScreenResolution& masterScreenResolution,
    SetScreenResolution setScreenResolution)
try
{
    auto keyboard = std::make_unique<event_vendor::KeyboardSender>();
    auto mouse = new event_vendor::MouseSender(std::make_unique<commons::CursorGuard>());
    auto port = std::string("10000");
    auto socket = std::make_unique<connection::Socket>(address, port);
    vendor =
        std::make_unique<app_management::Vendor>(std::move(keyboard), mouse, std::move(socket), setScreenResolution);
    vendor->start(masterScreenResolution);
}
catch (std::exception e)
{
    std::clog << e.what() << std::endl;
}

void App::setContactPoints(
    const std::pair<internal_types::Point, internal_types::Point>& contactPoints,
    const internal_types::Point& diffPointForSend,
    const internal_types::Point& diffPointForReceive)
{
    if (vendor)
    {
        vendor->setContactPoints(contactPoints, diffPointForSend, diffPointForReceive);
    }
    else
    {
        std::cerr << "Vendor doesn't exist" << std::endl;
    }
}

void App::stop()
{
    vendor->stop();
}
} // namespace app_management
