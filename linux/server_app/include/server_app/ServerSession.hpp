#include <array>
#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>
#include <cstddef>

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
    std::array<std::byte, 2> charPtr = {std::byte{0}, std::byte{0}};
};
} // namespace server_app
