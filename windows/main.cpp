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
            endpoints = resolver.resolve("192.168.0.22", "54000");
        }
        else
        {
            std::cout << "111111111" << std::endl;
            std::cout << argv[0] << std::endl;
            endpoints = resolver.resolve(argv[1], "54000");
            std::cout << "222222222" << std::endl;
        }
        Client c(io_context, endpoints);

        std::thread t([&io_context]() { io_context.run(); });

        auto pressedKeyCallback = [&c](std::array<std::byte, 2> key) { c.send(key); };

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
