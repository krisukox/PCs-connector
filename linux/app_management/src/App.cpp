#include "app_management/App.hpp"
#include "internal_types/Deserializer.hpp"
#include "key_management/KeyHandlerSelector.hpp"

namespace app_management
{
using boost::asio::ip::tcp;

App::App(int argc, char* argv[]) : endpoint{tcp::v4(), static_cast<unsigned short>(std::atoi("10000"))}
{
    Display* display{XOpenDisplay(nullptr)};

    servers.emplace_back(
        io_context,
        endpoint,
        key_management::KeyHandlerSelector{display}.select(argc, argv),
        std::make_unique<internal_types::Deserializer>(display));

    io_context.run();
}
} // namespace app_management
