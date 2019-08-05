#pragma once

#include <boost/asio/io_context.hpp>
#include <list>
#include "server_app/Server.hpp"

namespace app_management
{
using boost::asio::ip::tcp;

class App
{
public:
    App(int, char* []);

private:
    tcp::endpoint endpoint;
    boost::asio::io_context io_context;
    std::list<server_app::Server> servers;
};
} // namespace app_management
