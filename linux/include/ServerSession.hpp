#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>
#include "../include/TestKey.hpp"
#include "IKey.hpp"

using boost::asio::ip::tcp;

class ServerSession : public std::enable_shared_from_this<ServerSession>
{
public:
    ServerSession(tcp::socket, std::shared_ptr<IKey>);
    void start();

private:
    void readBody();

    tcp::socket socket;
    std::shared_ptr<IKey> keyHandler;
    char* charPtr = new char[2];
};
