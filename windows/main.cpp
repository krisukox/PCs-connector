#pragma comment(lib, "ws2_32.lib")

//#include "Client.hpp"

//#include <stdexcept>
//#include <iostream>

//#include <boost/asio.hpp>

// int main()
//{
//    try {
//        Client client{};
//    } catch (std::runtime_error e) {
//        std::cout<<e.what();
//    }

//    boost::asio::ip::address_v4 add{128};
//    std::cout<<add<<std::endl;
//}

#include <boost/asio.hpp>
#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include "Client.hpp"
#include "Keyboard.hpp"
#include "chat_message.hpp"

using boost::asio::ip::tcp;

typedef std::deque<chat_message> chat_message_queue;

int main(int, char*[])
{
    std::unique_ptr<Keyboard> keyboard;
    try
    {
        //        if (argc != 3)
        //        {
        //            std::cerr << "Usage: chat_client <host> <port>\n";
        //            return 1;
        //        }

        boost::asio::io_context io_context;

        tcp::resolver resolver(io_context);
        auto endpoints = resolver.resolve("192.168.0.22", "54000");
        chat_client c(io_context, endpoints);

        std::thread t([&io_context]() { io_context.run(); });

        keyboard = std::make_unique<Keyboard>([&c](const unsigned& key) {
            chat_message msg;
            msg.body_length(1);
            msg.body()[0] = static_cast<char>(key);
            //            std::memcpy(msg.body(), line, msg.body_length());
            std::cout << "HEADER0" << std::endl;
            msg.encode_header();
            std::cout << "HEADER3" << std::endl;
            c.do_write();
        });
        while (true)
            ;

        //        char line[chat_message::max_body_length + 1];
        //        std::cout << "TAK" << std::endl;
        //        while (std::cin.getline(line, chat_message::max_body_length + 1))
        //        {
        //            chat_message msg;
        //            msg.body_length(std::strlen(line));
        //            std::memcpy(msg.body(), line, msg.body_length());
        //            msg.encode_header();
        //            c.write(msg);
        //        }

        c.close();
        t.join();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
