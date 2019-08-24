#pragma once

#include <boost/asio.hpp>
#include "ServerAppTypes.hpp"

namespace server_app
{
class IReceiver
{
public:
    virtual void asyncRead(boost::asio::ip::tcp::socket&, Buffer&, Handler&&) = 0;
    virtual ~IReceiver() = default;
};
} // namespace server_app
