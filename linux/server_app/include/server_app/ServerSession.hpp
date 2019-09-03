#pragma once

#include <array>
#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>
#include <cstddef>
#include "ServerAppTypes.hpp"
#include "internal_types/Deserializer.hpp"

namespace key_management
{
class IKey;
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
        std::shared_ptr<IReceiver>,
        std::unique_ptr<internal_types::IDeserializer>);
    void start();

private:
    void onMessage(boost::system::error_code, std::size_t);
    void readBody();

    tcp::socket socket;
    std::shared_ptr<key_management::IKey> keyHandler;
    std::shared_ptr<IReceiver> receiver;
    std::unique_ptr<internal_types::IDeserializer> deserilizer;
    Buffer buffer = {std::byte{0}, std::byte{0}};

    unsigned count = 0;
};
} // namespace server_app
