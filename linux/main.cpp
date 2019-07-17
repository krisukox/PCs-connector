//#include "include/FakeKey.hpp"
#include "include/Server.hpp"
//#include <cstring>
//#include <boost/asio.hpp>
//#include <iostream>
#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>
#include <cstdlib>
#include <deque>
#include <iostream>
#include <list>
#include <memory>
#include <set>
#include <utility>
#include "include/chat_message.hpp"

// using boost::asio::ip::tcp;

////----------------------------------------------------------------------

// typedef std::deque<chat_message> chat_message_queue;

////----------------------------------------------------------------------

// class chat_session : public std::enable_shared_from_this<chat_session>
//{
// public:
//    chat_session(tcp::socket socket, FakeKey& _fakeKey) : socket_(std::move(socket)), fakeKey{_fakeKey} {}

//    void start()
//    {
//        std::cout << "start" << std::endl;
//        readBody();
//    }

// private:
//    void readBody()
//    {
//        std::cout << "READ HEADER" << std::endl;
//        auto self(shared_from_this());
//        boost::asio::async_read(
//            socket_, boost::asio::buffer(charPtr, 2), [self](boost::system::error_code ec, std::size_t /*length*/) {
//                std::cout << "header >>" << self->charPtr[0] << self->charPtr[1] << std::endl;
//                if (!ec && self->read_msg_.decode_header())
//                {
//                    std::cout << "JEST W DO_READ_BODY\n";
//                    self->readBody();
//                }
//                else
//                {
//                    // LEAVE
//                }
//            });
//    }

//    tcp::socket socket_;
//    chat_message read_msg_;
//    chat_message_queue write_msgs_;
//    FakeKey& fakeKey;
//    char* charPtr = new char[1];
//};

////----------------------------------------------------------------------

// class chat_server
//{
// public:
//    chat_server(boost::asio::io_context& io_context, const tcp::endpoint& endpoint, FakeKey& _fakeKey)
//        : acceptor_(io_context, endpoint), fakeKey{_fakeKey}
//    {
//        do_accept();
//    }

// private:
//    void do_accept()
//    {
//        acceptor_.async_accept([this](boost::system::error_code ec, tcp::socket socket) {
//            if (!ec)
//            {
//                std::make_shared<chat_session>(std::move(socket), fakeKey)->start();
//            }

//            do_accept();
//        });
//    }

//    tcp::acceptor acceptor_;
//    FakeKey& fakeKey;
//};

//----------------------------------------------------------------------

int main(int, char* [])
{
    try
    {
        //        if (argc < 2)
        //        {
        //            std::cerr << "Usage: chat_server <port> [<port> ...]\n";
        //            return 1;
        //        }

        boost::asio::io_context io_context;

        std::list<chat_server> servers;
        //        for (int i = 1; i < argc; ++i)
        //        {
        FakeKey fakeKey;
        tcp::endpoint endpoint(tcp::v4(), std::atoi("54000"));
        servers.emplace_back(io_context, endpoint, fakeKey);
        //        }

        io_context.run();
        std::cout << "KONIEC" << std::endl;
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
