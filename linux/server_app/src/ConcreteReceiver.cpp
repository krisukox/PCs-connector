#include "server_app/ConcreteReceiver.hpp"
#include <boost/asio.hpp>

namespace server_app
{
void ConcreteReceiver::asyncRead(boost::asio::ip::tcp::socket& readerSocket, Buffer& buffer, Handler&& handler)
{
    boost::asio::async_read(readerSocket, boost::asio::buffer(buffer, 2), handler);
}

ConcreteReceiver::~ConcreteReceiver() = default;

} // namespace server_app
