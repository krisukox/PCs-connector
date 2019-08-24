#include "app_management/App.hpp"
#include <iostream>
#include "key_management/FakeKey.hpp"
#include "key_management/KeyHandlerSelector.hpp"
#include "key_management/TestKey.hpp"

namespace app_management
{
using boost::asio::ip::tcp;

App::App(int argc, char* argv[]) : endpoint{tcp::v4(), static_cast<unsigned short>(std::atoi("10000"))}
{
    servers.emplace_back(io_context, endpoint, key_management::KeyHandlerSelector{}.select(argc, argv));

    io_context.run();
}
} // namespace app_management
