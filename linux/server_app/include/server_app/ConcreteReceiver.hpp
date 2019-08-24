#pragma once

#include "IReceiver.hpp"

namespace server_app
{
class ConcreteReceiver : public IReceiver
{
public:
    ConcreteReceiver() = default;
    ~ConcreteReceiver() override;

    void asyncRead(boost::asio::ip::tcp::socket&, Buffer&, Handler&&) override;
};
} // namespace server_app
