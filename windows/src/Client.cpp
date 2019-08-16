#include "Client.hpp"
#include <array>
#include <boost/asio.hpp>
#include <cstddef>
#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>

using boost::asio::ip::tcp;

Client::Client(boost::asio::io_context& io_context, const tcp::resolver::results_type& endpoints)
    : ioContext{io_context}, sessionSocket{ioContext}
{
    connect(endpoints);
}

void Client::close()
{
    boost::asio::post(ioContext, [this]() { sessionSocket.close(); });
}

void Client::connect(const tcp::resolver::results_type& endpoints)
{
    boost::asio::async_connect(sessionSocket, endpoints, [](boost::system::error_code ec, tcp::endpoint) {
        if (!ec)
        {
            std::cout << "Connection works properly" << std::endl;
        }
    });
}

void Client::send(std::array<std::byte, 2> key)
{
    //    std::cerr << static_cast<int>(key[0]) << " " << static_cast<int>(key[1]) << std::endl;
    boost::asio::async_write(
        sessionSocket, boost::asio::buffer(key, 2), [this](boost::system::error_code ec, std::size_t /*length*/) {
            if (!ec)
            {
                // LATER CHECK IF VECTOR OF MSGS HAS ANY MSGS LEFT
            }
            else
            {
                sessionSocket.close();
            }
        });
}
