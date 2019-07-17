#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>

#include <deque>
#include <iostream>
#include <list>
#include <memory>
#include "FakeKey.hpp"
#include "chat_message.hpp"

using boost::asio::ip::tcp;

//----------------------------------------------------------------------

typedef std::deque<chat_message> chat_message_queue;

//----------------------------------------------------------------------

class chat_session : public std::enable_shared_from_this<chat_session>
{
public:
    chat_session(tcp::socket socket, FakeKey& _fakeKey) : socket_(std::move(socket)), fakeKey{_fakeKey} {}

    void start()
    {
        std::cout << "start" << std::endl;
        readBody();
    }

private:
    void readBody()
    {
        std::cout << "READ HEADER" << std::endl;
        auto self(shared_from_this());
        boost::asio::async_read(
            socket_, boost::asio::buffer(charPtr, 2), [self](boost::system::error_code ec, std::size_t /*length*/) {
                std::cout << "header >>" << self->charPtr[0] << self->charPtr[1] << std::endl;
                if (!ec && self->read_msg_.decode_header())
                {
                    std::cout << "JEST W DO_READ_BODY\n";
                    self->readBody();
                }
                else
                {
                    // LEAVE
                }
            });
    }

    tcp::socket socket_;
    chat_message read_msg_;
    chat_message_queue write_msgs_;
    FakeKey& fakeKey;
    char* charPtr = new char[1];
};

//----------------------------------------------------------------------

class chat_server
{
public:
    chat_server(boost::asio::io_context& io_context, const tcp::endpoint& endpoint, FakeKey& _fakeKey);

private:
    void do_accept();

    tcp::acceptor acceptor_;
    FakeKey& fakeKey;
};
