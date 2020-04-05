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
    std::cout << "App::connect 11" << std::endl;
    //    setScreenResolution(internal_types::ScreenResolution{1080, 1920});
    auto keyboard = std::make_unique<event_vendor::KeyboardSender>();
    std::cout << "App::connect 22" << std::endl;
    auto mouse = std::make_unique<event_vendor::MouseSender>(std::make_unique<commons::CursorGuard>());
    auto port = std::string("10000");
    std::cout << "App::connect 33" << std::endl;
    /*auto */ socket = std::make_unique<connection::Socket>(address, port);
    std::cout << "App::connect 44" << std::endl;
    //    setScreenResolution(internal_types::ScreenResolution{1080, 1920});
    std::cout << "App::connect 55" << std::endl;
    //    internal_types::Serializer serializer;
    //    boost::asio::async_write(
    //        socket->socket,
    //        boost::asio::buffer(serializer.encode<>(masterScreenResolution), 5),
    //        [this](boost::system::error_code ec, std::size_t) {});
    //    connection::Receiver::SuccessfulCallback<internal_types::ScreenResolution> successfulCallback =
    //        [this](internal_types::ScreenResolution screenResolution) { std::cout << "RECEIVE SYNCHRONISED" <<
    //        std::endl; };
    //    connection::Receiver::UnsuccessfulCallback unsuccessfulCallback = [](boost::system::error_code ec) {};

    //    socket->synchronizedReceive(successfulCallback, unsuccessfulCallback);

    //    connection::Receiver::SuccessfulCallback<internal_types::ScreenResolution> successfulCallback1 =
    //        [this](const internal_types::ScreenResolution& event) {
    //            std::cout << "Vendor::receiveEvent RECEIVE ASYNC" << std::endl;
    //        };
    //    connection::Receiver::UnsuccessfulCallback unsuccessfulCallback1 = [this](boost::system::error_code) {
    //        std::cerr << "Unsuccessful event receive" << std::endl;
    //    };
    //    socket->receive(successfulCallback1, unsuccessfulCallback1);
    //    socket->start();

    vendor = std::make_unique<app_management::Vendor>(
        std::move(keyboard), std::move(mouse), std::move(socket), setScreenResolution, masterScreenResolution);
    vendor->start(masterScreenResolution);

    //    vendorThread = std::thread(&Vendor::startCatchingEvents, vendor);
    //    vendor->startReceivingEvents();
    //    socket->connect(address, port, [this, masterScreenResolution]() {
    //        initializeVendor(masterScreenResolution);
    //        vendorThread = std::thread(&Vendor::startCatchingEvents, vendor);
    //        vendor->startReceivingEvents();
    //        //        socket->getIoContext().run();
    //        //        t.join();
    //    });

    //    initializeVendor(masterScreenResolution);
    //    std::thread t(&Vendor::startCatchingEvents, vendor);
    //    vendor->startReceivingEvents();
    //    socket->getIoContext().run();
    //    t.join();

    //    internal_types::Serializer serializer;

    //    boost::asio::async_write(
    //        socket->socket,
    //        boost::asio::buffer(serializer.encode<>(masterScreenResolution), 5),
    //        [this](boost::system::error_code ec, std::size_t) {
    //            if (!ec)
    //            {
    //                // LATER CHECK IF VECTOR OF MSGS HAS ANY MSGS LEFT
    //            }
    //            else
    //            {
    //                //                socket.close();
    //            }
    //        });
    //    //    connection::Receiver::SuccessfulCallback<internal_types::ScreenResolution> successfulCallback =
    //    //        [this](internal_types::ScreenResolution screenResolution) {
    //    setScreenResolution(screenResolution); };
    //    //    connection::Receiver::UnsuccessfulCallback unsuccessfulCallback = [](boost::system::error_code ec)
    //    {};

    //    if (socket->socket.receive(boost::asio::buffer(buffer, 5)) == 5)
    //    {
    //        std::cout << "RECEIVED SYNCHRONISED" << std::endl;
    //    }

    //    connection::Receiver::SuccessfulCallback<internal_types::ScreenResolution> successfulCallback =
    //        [this](const internal_types::ScreenResolution& event) {
    //            std::cout << "Vendor::receiveEvent ASYNC" << std::endl;
    //            //            keyboard->changeState();
    //            //            mouse->changeMouseState(
    //            //
    //            std::get<internal_types::MouseChangePositionEvent>(std::get<internal_types::MouseEvent>(event)));
    //            //            startReceivingEvents();
    //        };
    //    connection::Receiver::UnsuccessfulCallback unsuccessfulCallback = [this](boost::system::error_code) {
    //        std::cerr << "Unsuccessful event receive" << std::endl;
    //        //        stopApp();
    //    };
    //    socket->socket.async_receive(
    //        boost::asio::buffer(buffer, 5),
    //        [this, successfulCallback, unsuccessfulCallback](boost::system::error_code errorCode, std::size_t
    //        size) {
    //            if (size > 0 && !errorCode)
    //            {
    //                std::cout << "Vendor::receiveEvent ASYNC" << std::endl;
    //                //                handleReceivedData(successfulCallback, unsuccessfulCallback);
    //            }
    //            else
    //            {
    //                //                unsuccessfulCallback(errorCode);
    //            }
    //        });
    //    socket->start();
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
    std::cout << "App::setContactPoints11" << std::endl;
    vendor->setContactPoints(contactPoints, diffPointForSend, diffPointForReceive);
    std::cout << "App::setContactPoints22" << std::endl;
}

void App::initializeVendor(const internal_types::ScreenResolution& masterScreenResolution)
{
    //    auto stopAppCallback = [this] { socket->close(); };

    //    auto receiver =
    //        std::make_shared<connection::Receiver>(socket->value(), std::make_unique<internal_types::Deserializer>());
    //    auto sender = std::make_shared<connection::Sender>(socket->value());

    exchangeScreenResolution(masterScreenResolution);

    //    auto keyboard = std::make_shared<event_vendor::KeyboardSender>(sender);
    //    auto mouse = std::make_shared<event_vendor::MouseSender>(sender, cursorGuard);

    //    vendor = std::make_shared<app_management::Vendor>(keyboard, mouse, receiver, stopAppCallback);
}

void App::exchangeScreenResolution(const internal_types::ScreenResolution& masterScreenResolution)
{
    //    sender->send(masterScreenResolution);
    //    connection::Receiver::SuccessfulCallback<internal_types::ScreenResolution> successfulCallback =
    //        [this](internal_types::ScreenResolution screenResolution) { setScreenResolution(screenResolution); };
    //    connection::Receiver::UnsuccessfulCallback unsuccessfulCallback = [](boost::system::error_code ec) {};

    //    receiver->synchronizedReceive(successfulCallback, unsuccessfulCallback);
}
} // namespace app_management
