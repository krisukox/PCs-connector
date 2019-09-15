#pragma once

#include <boost/asio.hpp>
#include "ServerAppTypes.hpp"
#include "internal_types/CommonTypes.hpp"

namespace server_app
{
class IReceiver
{
public:
    virtual void asyncRead(boost::asio::ip::tcp::socket&, internal_types::Buffer&, Handler&&) = 0;
    virtual ~IReceiver() = default;
};
} // namespace server_app
