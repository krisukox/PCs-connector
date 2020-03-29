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

App::App(std::shared_ptr<commons::CursorGuard>&& cursorGuard, SetScreenResolution&& _setScreenResolution)
    : commons::IApp(std::move(cursorGuard), std::move(_setScreenResolution))
    , socket{std::make_unique<connection::Socket>()}
{
}

void App::connect(
    const boost::asio::ip::address& address,
    const internal_types::ScreenResolution& masterScreenResolution)
try
{
    auto port = std::string("10000");
    socket->connect(address, port);

    initializeVendor(masterScreenResolution);
    std::thread t(&Vendor::startCatchingEvents, vendor);
    vendor->startReceivingEvents();
    socket->getIoContext().run();
    t.join();
}
catch (std::exception e)
{
    std::clog << e.what() << std::endl;
}

void App::initializeVendor(const internal_types::ScreenResolution& masterScreenResolution)
{
    auto stopAppCallback = [this] { socket->close(); };

    auto receiver =
        std::make_shared<connection::Receiver>(socket->value(), std::make_unique<internal_types::Deserializer>());
    auto sender = std::make_shared<connection::Sender>(socket->value());

    exchangeScreenResolution(receiver, sender, masterScreenResolution);

    auto keyboard = std::make_shared<event_vendor::KeyboardSender>(sender);
    auto mouse = std::make_shared<event_vendor::MouseSender>(sender, cursorGuard);

    vendor = std::make_shared<app_management::Vendor>(keyboard, mouse, receiver, stopAppCallback);
}

void App::exchangeScreenResolution(
    std::shared_ptr<connection::Receiver> receiver,
    std::shared_ptr<connection::Sender> sender,
    const internal_types::ScreenResolution& masterScreenResolution)
{
    sender->send(masterScreenResolution);
    connection::Receiver::SuccessfulCallback<internal_types::ScreenResolution> successfulCallback =
        [this](internal_types::ScreenResolution screenResolution) { setScreenResolution(screenResolution); };
    connection::Receiver::UnsuccessfulCallback unsuccessfulCallback = [](boost::system::error_code ec) {};

    receiver->synchronizedReceive(successfulCallback, unsuccessfulCallback);
}
} // namespace app_management
