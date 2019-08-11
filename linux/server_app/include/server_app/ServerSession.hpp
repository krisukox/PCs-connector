#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>

namespace key_management
{
class IKey;
}

namespace server_app
{
using boost::asio::ip::tcp;

class ServerSession : public std::enable_shared_from_this<ServerSession>
{
public:
    ServerSession(tcp::socket, std::shared_ptr<key_management::IKey>);
    void start();
    void onMessage(boost::system::error_code, std::size_t);

private:
    void readBody();

    tcp::socket socket;
    std::shared_ptr<key_management::IKey> keyHandler;
    char* charPtr = new char[2];
};
} // namespace server_app
