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
        boost::asio::io_context&,
        std::function<void()>&&);

    void start();

private:
    void startCatchingEvents();
    void read();

    std::shared_ptr<event_vendor::KeyboardSender> keyboard;
    std::shared_ptr<event_vendor::MouseSender> mouse;
    std::shared_ptr<connection::IReceiver> receiver;
    boost::asio::io_context& ioContext;
    std::function<void()>&& stopAppCallback;
};
} // namespace app_management
