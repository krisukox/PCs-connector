#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>
#include <iostream>
#include <list>
#include "include/FakeKey.hpp"
#include "include/Server.hpp"

int main(int, char* [])
{
    try
    {
        boost::asio::io_context io_context;

        std::list<Server> servers;

        FakeKey fakeKey;
        tcp::endpoint endpoint(tcp::v4(), static_cast<unsigned short>(std::atoi("54000")));
        servers.emplace_back(io_context, endpoint, fakeKey);

        io_context.run();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
