#pragma once

#include <gmock/gmock.h>
#include <optional>
#include "server_app/IReceiver.hpp"

namespace mocks
{
struct ReceiverMock : server_app::IReceiver
{
    void asyncRead(boost::asio::ip::tcp::socket&, server_app::Buffer& byteBuffer, server_app::Handler&& handler)
        override
    {
        startAsyncRead();

        returnBuffer = byteBuffer;
        callHandler = std::move(handler);
    }

    void setReturnBuffer(server_app::Buffer byteBuffer) { returnBuffer->get() = byteBuffer; }

    std::optional<std::reference_wrapper<server_app::Buffer>> returnBuffer;
    server_app::Handler callHandler;

    MOCK_METHOD0(startAsyncRead, void());
};
} // namespace mocks
