#pragma once

#include <array>
#include <boost/asio.hpp>
#include <cstddef>
#include <functional>

namespace server_app
{
using Buffer = std::array<std::byte, 2>;
using Handler = std::function<void(boost::system::error_code, std::size_t)>;
} // namespace server_app
