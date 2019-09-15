#pragma once

#include <boost/asio.hpp>
#include <functional>

namespace server_app
{
using Handler = std::function<void(boost::system::error_code, std::size_t)>;
} // namespace server_app
