#include <boost/asio.hpp>
#include <cstddef>
#include <iostream>
#include <thread>
#include "Client.hpp"
#include "Keyboard.hpp"

using boost::asio::ip::tcp;

int main(int, char*[])
{
    try
    {
        boost::asio::io_context io_context;

        tcp::resolver resolver(io_context);
        auto endpoints = resolver.resolve("192.168.0.22", "54000");
        Client c(io_context, endpoints);

        std::thread t([&io_context]() { io_context.run(); });

        auto pressedKeyCallback = [&c](const unsigned key) { c.send(key); };

        auto stopAppCallback = [&c, &t]() {
            c.close();
            t.join();
        };

        auto keyboard = std::make_unique<Keyboard>(std::move(pressedKeyCallback), std::move(stopAppCallback));
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
