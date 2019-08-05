#include "app_management/App.hpp"
#include <iostream>
#include "key_management/FakeKey.hpp"
#include "key_management/TestKey.hpp"

namespace app_management
{
using boost::asio::ip::tcp;

App::App(int argc, char* argv[]) : endpoint{tcp::v4(), static_cast<unsigned short>(std::atoi("54000"))}
{
    std::shared_ptr<key_management::IKey> keyHandler;
    if (argc == 2 && !strcmp(argv[1], "test"))
    {
        std::clog << "test mode started" << std::endl;
        keyHandler = std::make_shared<key_management::TestKey>();
    }
    else if (argc == 1)
    {
        std::clog << "release mode started" << std::endl;
        keyHandler = std::make_shared<key_management::FakeKey>();
    }
    else
    {
        throw std::invalid_argument("not valid startup argument");
    }

    servers.emplace_back(io_context, endpoint, std::move(keyHandler));

    io_context.run();
}
} // namespace app_management
