#include <boost/asio.hpp>
#include <iostream>
#include <thread>
#include "Client.hpp"
#include "Keyboard.hpp"
//#include "Mouse.hpp"
#include "boost/asio/impl/read.hpp"
#include "mouse_management/MouseHandler.hpp"

// using boost::asio::ip::tcp;

int main(int argc, char* argv[])
{
    try
    {
        boost::asio::io_context io_context;

        tcp::resolver resolver(io_context);
        tcp::resolver::results_type endpoints;
        if (argc == 1)
        {
            endpoints = resolver.resolve("192.168.1.5", "10000");
        }
        else
        {
            endpoints = resolver.resolve(argv[1], "10000");
        }
        Client c(io_context, endpoints);
        io_context.run();

        auto pressedKeyCallback = [&c](std::array<std::byte, 5> key) { c.send(key); };

        auto stopAppCallback = [&c]() { c.close(); };
        auto keyboard = Keyboard(std::move(pressedKeyCallback), std::move(stopAppCallback));
        keyboard.start();
        auto mouse = Mouse(std::move(pressedKeyCallback), std::move(stopAppCallback));
        mouse.start();
        MSG msg;
        BOOL retVal;
        while ((retVal = GetMessage(&msg, nullptr, 0, 0)) != 0)
        {
            if (retVal == -1)
            {
                // handle the error and possibly exit
            }
        }
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }
    return 0;
}
