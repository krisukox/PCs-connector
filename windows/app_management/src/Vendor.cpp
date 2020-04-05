#include "app_management/Vendor.hpp"
#include <iostream>
#include "connection/Receiver.hpp"
#include "connection/Socket.hpp"
#include "event_vendor/KeyboardSender.hpp"
#include "event_vendor/MouseSender.hpp"

namespace app_management
{
Vendor::Vendor(
    std::unique_ptr<event_vendor::KeyboardSender> keyboardSender,
    std::unique_ptr<event_vendor::MouseSender> mouseSender,
    std::unique_ptr<connection::Socket> _socket,
    std::function<void(internal_types::ScreenResolution)> _setScreenResolution,
    const internal_types::ScreenResolution& masterScreenResolution)
    : keyboard{std::move(keyboardSender)}
    , mouse{std::move(mouseSender)}
    , socket{std::move(_socket)}
    , setScreenResolution{_setScreenResolution}
{
    std::cout << "Vendor::Vendor" << std::endl;

    //    socket->send(masterScreenResolution);

    //    connection::Receiver::SuccessfulCallback<internal_types::ScreenResolution> successfulCallback =
    //        [this](internal_types::ScreenResolution screenResolution) { std::cout << "RECEIVE SYNCHRONISED" <<
    //        std::endl; };
    //    connection::Receiver::UnsuccessfulCallback unsuccessfulCallback = [](boost::system::error_code ec) {};

    //    socket->synchronizedReceive(successfulCallback, unsuccessfulCallback);

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

    socket->send(masterScreenResolution);
    connection::Receiver::SuccessfulCallback<internal_types::ScreenResolution> successfulCallback =
        [this](internal_types::ScreenResolution screenResolution) {
            std::cout << "RECEIVE SYNCHRONISED" << std::endl; /* setScreenResolution(screenResolution);*/
        };
    connection::Receiver::UnsuccessfulCallback unsuccessfulCallback = [](boost::system::error_code ec) {};
    //    setScreenResolution(internal_types::ScreenResolution{1080, 1920});

    socket->synchronizedReceive(successfulCallback, unsuccessfulCallback);

    //    eventCatchingThread = std::thread(&Vendor::startCatchingEvents, this);
    receiveEvent();
    socket->start();
    std::cout << "Vendor::Vendor" << std::endl;
}

void Vendor::start(const internal_types::ScreenResolution& masterScreenResolution)
{
    socket->send(masterScreenResolution);
    connection::Receiver::SuccessfulCallback<internal_types::ScreenResolution> successfulCallback =
        [this](internal_types::ScreenResolution screenResolution) {
            std::cout << "RECEIVE SYNCHRONISED" << std::endl;
            setScreenResolution(screenResolution);
        };
    connection::Receiver::UnsuccessfulCallback unsuccessfulCallback = [](boost::system::error_code ec) {};

    socket->synchronizedReceive(successfulCallback, unsuccessfulCallback);
    //    socket->synchronizedReceive(successfulCallback, unsuccessfulCallback);

    //    eventCatchingThread = std::thread(&Vendor::startCatchingEvents, this);
    receiveEvent();
    socket->start();
    std::cout << "Vendor::Vendor" << std::endl;
}

Vendor::~Vendor()
{
    eventCatchingThread.join();
}

void Vendor::setContactPoints(
    const std::pair<internal_types::Point, internal_types::Point>& contactPoints,
    const internal_types::Point& diffPointForSend,
    const internal_types::Point& diffPointForReceive)
{
    mouse->setContactPoints(contactPoints, diffPointForSend, diffPointForReceive);
}

void Vendor::startReceivingEvents()
{
    receiveEvent();
}

void Vendor::receiveEvent()
{
    connection::Receiver::SuccessfulCallback<internal_types::ScreenResolution> successfulCallback =
        [this](const internal_types::ScreenResolution& event) {
            std::cout << "Vendor::receiveEvent RECEIVE ASYNC" << std::endl;
            //            socket->send(internal_types::ScreenResolution{1080, 1920});
            //            keyboard->changeState();
            //            mouse->changeMouseState(
            //                std::get<internal_types::MouseChangePositionEvent>(std::get<internal_types::MouseEvent>(event)));
            receiveEvent();
        };
    connection::Receiver::UnsuccessfulCallback unsuccessfulCallback = [this](boost::system::error_code) {
        std::cerr << "Unsuccessful event receive" << std::endl;
        stopApp();
    };
    socket->receive(successfulCallback, unsuccessfulCallback);
}

void Vendor::startCatchingEvents()
{
    keyboard->start([this](const internal_types::KeyEvent& keyEvent) { socket->send(keyEvent); });
    mouse->start(std::bind(&Vendor::changeKeyboardState, this), [this](const internal_types::MouseEvent& mouseEvent) {
        socket->send(mouseEvent);
    });

    MSG msg;
    BOOL retVal;
    while ((retVal = GetMessage(&msg, nullptr, 0, 0)) != 0)
    {
        if (retVal == -1)
        {
            //            stopAppCallback();
        }
    }
}

void Vendor::stopApp()
{
    PostMessage(nullptr, WM_QUIT, 0, 0);
    //    stopAppCallback();
}

void Vendor::changeKeyboardState()
{
    keyboard->changeState();
}
} // namespace app_management
