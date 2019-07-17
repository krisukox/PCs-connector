#include "include/FakeKey.hpp"
//#include "include/Server.hpp"
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

// int main()
//{
//    boost::asio::ip::address_v4 add{128};
//    std::cout<<add<<std::endl;

//    Server server;

//}

using boost::asio::ip::tcp;

//----------------------------------------------------------------------

typedef std::deque<chat_message> chat_message_queue;

//----------------------------------------------------------------------

class chat_participant
{
public:
    virtual ~chat_participant() {}
    virtual void deliver(const chat_message& msg) = 0;
};

typedef std::shared_ptr<chat_participant> chat_participant_ptr;

//----------------------------------------------------------------------

class chat_room
{
public:
    void join(chat_participant_ptr participant)
    {
        participants_.insert(participant);
        for (auto msg : recent_msgs_)
            participant->deliver(msg);
    }

    void leave(chat_participant_ptr participant) { participants_.erase(participant); }

    void deliver(const chat_message& msg)
    {
        recent_msgs_.push_back(msg);
        while (recent_msgs_.size() > max_recent_msgs)
            recent_msgs_.pop_front();

        for (auto participant : participants_)
            participant->deliver(msg);
    }

    void cos() {}

private:
    std::set<chat_participant_ptr> participants_;
    enum
    {
        max_recent_msgs = 100
    };
    chat_message_queue recent_msgs_;
};

//----------------------------------------------------------------------

class chat_session : public chat_participant, public std::enable_shared_from_this<chat_session>
{
public:
    chat_session(tcp::socket socket, chat_room& room, FakeKey& _fakeKey)
        : socket_(std::move(socket)), room_(room), fakeKey{_fakeKey}
    {
    }

    void start()
    {
        std::cout << "start" << std::endl;
        room_.join(shared_from_this());
        readBody();
    }

    void deliver(const chat_message& msg)
    {
        bool write_in_progress = !write_msgs_.empty();
        write_msgs_.push_back(msg);
        if (!write_in_progress)
        {
            do_write();
        }
    }

private:
    void readBody()
    {
        std::cout << "READ HEADER" << std::endl;
        auto self(shared_from_this());
        boost::asio::async_read(
            socket_,
            boost::asio::buffer(charPtr, 2),
            [this, self](boost::system::error_code ec, std::size_t /*length*/) {
                std::cout << "header >>" << charPtr[0] << charPtr[1] << std::endl;
                if (!ec && read_msg_.decode_header())
                {
                    std::cout << "JEST W DO_READ_BODY\n";
                    readBody();
                }
                else
                {
                    room_.leave(shared_from_this());
                }
            });
    }

    //    void do_read_body()
    //    {
    //        std::cout << "READ BODY" << std::endl;
    //        std::cout << "LENGTH: " << read_msg_.body_length() << std::endl;
    //        std::cout << "LENGTH: " << read_msg_.length() << std::endl;
    //        auto self(shared_from_this());
    //        boost::asio::async_read(
    //            socket_,
    //            boost::asio::buffer(read_msg_.body(), 0),
    //            [this, self](boost::system::error_code ec, std::size_t /*length*/) {
    //                std::cout << "cos" << std::endl;
    //                //                std::cout << charPtr << std::endl;
    //                if (!ec)
    //                {
    //                    //                    std::cout.write(read_msg_.body(), read_msg_.body_length());
    //                    fakeKey.pressKey(static_cast<u_int16_t>(read_msg_.body()[0]));
    //                    std::cout << "DOSTALO" << std::endl;
    //                    //                                        room_.deliver(read_msg_);
    //                    do_read_header();
    //                }
    //                else
    //                {
    //                    room_.leave(shared_from_this());
    //                }
    //            });
    //    }

    void do_write()
    {
        auto self(shared_from_this());
        boost::asio::async_write(
            socket_,
            boost::asio::buffer(write_msgs_.front().data(), write_msgs_.front().length()),
            [this, self](boost::system::error_code ec, std::size_t /*length*/) {
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
                    room_.leave(shared_from_this());
                }
            });
    }

    tcp::socket socket_;
    chat_room& room_;
    chat_message read_msg_;
    chat_message_queue write_msgs_;
    FakeKey& fakeKey;
    char* charPtr = new char[1];
};

//----------------------------------------------------------------------

class chat_server
{
public:
    chat_server(boost::asio::io_context& io_context, const tcp::endpoint& endpoint, FakeKey& _fakeKey)
        : acceptor_(io_context, endpoint), fakeKey{_fakeKey}
    {
        do_accept();
    }

private:
    void do_accept()
    {
        acceptor_.async_accept([this](boost::system::error_code ec, tcp::socket socket) {
            if (!ec)
            {
                std::make_shared<chat_session>(std::move(socket), room_, fakeKey)->start();
            }

            do_accept();
        });
    }

    tcp::acceptor acceptor_;
    chat_room room_;
    FakeKey& fakeKey;
};

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
