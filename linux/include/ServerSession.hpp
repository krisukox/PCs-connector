#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>
#include "FakeKey.hpp"

using boost::asio::ip::tcp;

class ServerSession : public std::enable_shared_from_this<ServerSession>
{
public:
    ServerSession(tcp::socket, FakeKey&);

    void start();

private:
    void readBody();

    tcp::socket socket;
    FakeKey& fakeKey;
    char* charPtr = new char[2];
};
