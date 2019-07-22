#pragma once

#include <boost/asio.hpp>
#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include "Console.hpp"
#include "Keyboard.hpp"

using boost::asio::ip::tcp;

class Client
{
public:
    Client(boost::asio::io_context&, const tcp::resolver::results_type&);
    void close();
    void send(const unsigned);

private:
    void connect(const tcp::resolver::results_type&);

    boost::asio::io_context& ioContext;
    tcp::socket sessionSocket;
    Console _;
};
