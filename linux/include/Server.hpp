#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>
#include <memory>
#include "IKey.hpp"
#include "ServerSession.hpp"

using boost::asio::ip::tcp;

class Server
{
public:
    Server(boost::asio::io_context&, const tcp::endpoint&, std::shared_ptr<IKey>);

private:
    void do_accept(std::shared_ptr<IKey>);

    tcp::acceptor socketAcceptor;
};
