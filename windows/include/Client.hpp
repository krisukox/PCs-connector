#pragma once

#include <boost/asio.hpp>
#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include "Console.hpp"
#include "Keyboard.hpp"
#include "chat_message.hpp"

using boost::asio::ip::tcp;
typedef std::deque<chat_message> chat_message_queue;

class chat_client
{
public:
    chat_client(boost::asio::io_context&, const tcp::resolver::results_type&);
    void write(const chat_message& msg);
    void close();

private:
    void do_connect(const tcp::resolver::results_type&);
    void do_read_header();
    void do_read_body();
    void do_write();
    //    std::unique_ptr<Keyboard> keyboard;
    //    WSAData winSockData;
    //    std::string ipAddress = "192.168.0.22";
    //    u_short port = 54000;

    //    Console console{};
    //    SOCKET sock;
    boost::asio::io_context& io_context_;
    tcp::socket socket_;
    chat_message read_msg_;
    chat_message_queue write_msgs_;
};
