#include <boost/asio.hpp>
#include <iostream>
#include <thread>
#include "Client.hpp"
#include "Keyboard.hpp"
#include "boost/asio/impl/read.hpp"

using boost::asio::ip::tcp;

int main(int argc, char* argv[])
{
    try
    {
        boost::asio::io_context io_context;

        tcp::resolver resolver(io_context);
        tcp::resolver::results_type endpoints;
        if (argc == 1)
        {
            endpoints = resolver.resolve("192.168.0.22", "10000");
        }
        else
        {
            endpoints = resolver.resolve(argv[1], "54000");
        }
        Client c(io_context, endpoints);
        io_context.run();

        auto pressedKeyCallback = [&c](std::array<std::byte, 2> key) { c.send(key); };

        auto stopAppCallback = [&c]() { c.close(); };
        auto keyboard = Keyboard(std::move(pressedKeyCallback), std::move(stopAppCallback));
        std::thread t(&Keyboard::start, &keyboard);
        t.join();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }
    return 0;
}
