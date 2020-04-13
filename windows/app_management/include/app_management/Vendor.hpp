#pragma once

#include <boost/asio.hpp>
#include <functional>
#include <memory>
#include "internal_types/CommonTypes.hpp"
#include "internal_types/Point.hpp"
#include "internal_types/ScreenResolution.hpp"

namespace connection
{
class Socket;
} // namespace connection

namespace event_vendor
{
class KeyboardSender;
class MouseSender;
} // namespace event_vendor

namespace app_management
{
class Vendor : public std::enable_shared_from_this<Vendor>
{
public:
    Vendor(
        std::unique_ptr<event_vendor::KeyboardSender>,
        std::unique_ptr<event_vendor::MouseSender>,
        std::unique_ptr<connection::Socket>,
        std::function<void(internal_types::ScreenResolution)>);
    ~Vendor();

    void setContactPoints(
        const std::pair<internal_types::Point, internal_types::Point>&,
        const internal_types::Point&,
        const internal_types::Point&);

    void startCatchingEvents();

    void start(const internal_types::ScreenResolution&);

private:
    void registerForMouseChangePositionEvent();
    void changeKeyboardState();
    void handleReceivedEvent(const internal_types::Event&);

    std::unique_ptr<event_vendor::KeyboardSender> keyboard;
    std::unique_ptr<event_vendor::MouseSender> mouse;
    std::unique_ptr<connection::Socket> socket;
    std::function<void(internal_types::ScreenResolution)> setScreenResolution;

    std::thread eventCatchingThread;
    DWORD eventCatchingThreadId;
};
} // namespace app_management
