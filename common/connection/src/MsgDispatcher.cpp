#include "connection/MsgDispatcher.hpp"

namespace connection
{
MsgDispatcher::MsgDispatcher() : deserializer{} {}

void MsgDispatcher::handleReceivedData(const internal_types::Buffer& _buffer)
{
    auto decoded = deserializer.decode(_buffer);

    if (decoded)
    {
        std::visit([this](const auto& value) { handleReceivedType(value); }, decoded.value());
    }
    else
    {
        //        unsuccessfulCallback(boost::system::error_code());
    }
}
} // namespace connection
