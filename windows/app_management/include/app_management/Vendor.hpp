#pragma once

#include <boost/asio.hpp>
#include <functional>
#include <memory>

namespace connection
{
class IReceiver;
}

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
        std::shared_ptr<event_vendor::KeyboardSender>,
        std::shared_ptr<event_vendor::MouseSender>,
        std::shared_ptr<connection::IReceiver>,
        std::function<void()> stopAppCallback_);

    void startReceivingEvents();
    void startCatchingEvents();

private:
    void receiveEvent();
    void stopApp();
    void changeKeyboardState();

    std::shared_ptr<event_vendor::KeyboardSender> keyboard;
    std::shared_ptr<event_vendor::MouseSender> mouse;
    std::shared_ptr<connection::IReceiver> receiver;
    std::function<void()> stopAppCallback;
};
} // namespace app_management
