#include "Client.hpp"
#include <boost/asio.hpp>
#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include "chat_message.hpp"

using boost::asio::ip::tcp;

typedef std::deque<chat_message> chat_message_queue;

chat_client::chat_client(boost::asio::io_context& io_context, const tcp::resolver::results_type& endpoints)
    : io_context_(io_context), socket_(io_context)
{
    do_connect(endpoints);
}

void chat_client::write(const chat_message& msg)
{
    std::cout << "111111" << std::endl;
    boost::asio::post(io_context_, [this, msg]() {
        std::cout << "22222" << std::endl;
        bool write_in_progress = !write_msgs_.empty();
        std::cout << "33333" << std::endl;
        write_msgs_.push_back(msg);
        std::cout << "44444" << std::endl;
        if (!write_in_progress)
        {
            std::cout << "55555" << std::endl;
            do_write();
        }
    });
}

void chat_client::close()
{
    boost::asio::post(io_context_, [this]() { socket_.close(); });
}

void chat_client::do_connect(const tcp::resolver::results_type& endpoints)
{
    boost::asio::async_connect(socket_, endpoints, [this](boost::system::error_code ec, tcp::endpoint) {
        if (!ec)
        {
            //            while (true)
            //                ;
            //            do_read_header();
        }
    });
}

void chat_client::do_read_header()
{
    std::cout << "COS111111" << std::endl;
    boost::asio::async_read(
        socket_,
        boost::asio::buffer(read_msg_.data(), chat_message::header_length),
        [this](boost::system::error_code ec, std::size_t /*length*/) {
            if (!ec && read_msg_.decode_header())
            {
                do_read_body();
            }
            else
            {
                socket_.close();
            }
        });
}

void chat_client::do_read_body()
{
    std::cout << "COS222222" << std::endl;
    boost::asio::async_read(
        socket_,
        boost::asio::buffer(read_msg_.body(), read_msg_.body_length()),
        [this](boost::system::error_code ec, std::size_t /*length*/) {
            if (!ec)
            {
                std::cout << "DOSTALO" << std::endl;
                std::cout.write(read_msg_.body(), read_msg_.body_length());
                std::cout << "\n";
                do_read_header();
            }
            else
            {
                socket_.close();
            }
        });
}

void chat_client::do_write()
{
    //    chat_message chatMsg;
    chatMsg.body_length(1);
    chatMsg.body()[0] = 'K';
    chatMsg.encode_header();
    std::cout << "length: " << chatMsg.length() << std::endl;
    boost::asio::async_write(
        socket_,
        boost::asio::buffer(chatMsg.data(), chatMsg.length()),
        [this](boost::system::error_code ec, std::size_t /*length*/) {
            if (!ec)
            {
                write_msgs_.pop_front();
                if (!write_msgs_.empty())
                {
                    do_write();
                }
            }
            else
            {
                socket_.close();
            }
        });
}
