#pragma once

#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>

namespace connection
{
class IReceiver;
}

namespace event_consumer
{
class IKeyboardReceiver;
class IMouseReceiver;
} // namespace event_consumer

namespace app_management
{
using boost::asio::ip::tcp;

class Consumer : public std::enable_shared_from_this<Consumer>
{
public:
    Consumer(
        std::shared_ptr<event_consumer::IKeyboardReceiver>,
        std::shared_ptr<event_consumer::IMouseReceiver>,
        std::shared_ptr<connection::IReceiver>);
    void start();

private:
    void onMessage(const boost::system::error_code& ec, const std::size_t size);
    void readBody();

    std::shared_ptr<event_consumer::IKeyboardReceiver> keyReceiver;
    std::shared_ptr<event_consumer::IMouseReceiver> mouseReceiver;
    std::shared_ptr<connection::IReceiver> receiver;
};
} // namespace app_management
