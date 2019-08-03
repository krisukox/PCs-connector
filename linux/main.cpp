#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>
#include <cstdio>
#include <exception>
#include <iostream>
#include <list>
#include "include/FakeKey.hpp"
#include "include/Server.hpp"
#include "include/TestKey.hpp"

int main(int argc, char* argv[])
{
    try
    {
        std::shared_ptr<IKey> keyHandler;
        if (argc == 2 && !strcmp(argv[1], "test"))
        {
            std::clog << "test mode started" << std::endl;
            keyHandler = std::make_shared<TestKey>();
        }
        else if (argc == 1)
        {
            std::clog << "release mode started" << std::endl;
            keyHandler = std::make_shared<FakeKey>();
        }
        else
        {
            throw std::invalid_argument("not valid startup argument");
        }

        boost::asio::io_context io_context;
        std::list<Server> servers;

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
