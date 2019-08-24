#pragma once

#include <array>
#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>
#include <cstddef>
#include "ServerAppTypes.hpp"

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
    ServerSession(tcp::socket, std::shared_ptr<key_management::IKey>, std::shared_ptr<IReceiver>);
    void start();

private:
    void onMessage(boost::system::error_code, std::size_t);
    void readBody();

    tcp::socket socket;
    std::shared_ptr<key_management::IKey> keyHandler;
    std::shared_ptr<IReceiver> receiver;
    Buffer charPtr = {std::byte{0}, std::byte{0}};
};
} // namespace server_app
