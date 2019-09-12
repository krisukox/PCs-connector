#pragma once

#include <array>
#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>
#include <cstddef>
#include "ServerAppTypes.hpp"
#include "internal_types/CommonTypes.hpp"

namespace key_management
{
class IKey;
} // namespace key_management

namespace mouse_management
{
class IMouse;
} // namespace mouse_management

namespace internal_types
{
class IDeserializer;
}

namespace server_app
{
class IReceiver;

using boost::asio::ip::tcp;

class ServerSession
{
public:
    ServerSession(
        tcp::socket,
        std::shared_ptr<key_management::IKey>,
        std::shared_ptr<mouse_management::IMouse>,
        std::shared_ptr<IReceiver>,
        std::unique_ptr<internal_types::IDeserializer>);
    void start();

private:
    void onMessage(const boost::system::error_code& ec, const std::size_t size);
    void readBody();

    tcp::socket socket;
    std::shared_ptr<key_management::IKey> keyHandler;
    std::shared_ptr<mouse_management::IMouse> mouseHandler;
    std::shared_ptr<IReceiver> receiver;
    std::unique_ptr<internal_types::IDeserializer> deserilizer;

    internal_types::Buffer buffer;
};
} // namespace server_app
