#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>
#include <cstdio>
#include <exception>
#include <iostream>
#include <list>
#include "include/Server.hpp"
#include "key_management/FakeKey.hpp"
#include "key_management/TestKey.hpp"

using boost::asio::ip::tcp;

int main(int argc, char* argv[])
{
    try
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

        boost::asio::io_context io_context;
        std::list<server_app::Server> servers;

        tcp::endpoint endpoint(tcp::v4(), static_cast<unsigned short>(std::atoi("54000")));
        servers.emplace_back(io_context, endpoint, std::move(keyHandler));

        io_context.run();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
