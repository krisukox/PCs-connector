#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>
#include "FakeKey.hpp"

using boost::asio::ip::tcp;

class Server
{
public:
    Server(boost::asio::io_context&, const tcp::endpoint&, FakeKey&);

private:
    void do_accept();

    tcp::acceptor socketAcceptor;
    FakeKey& fakeKey;
};
