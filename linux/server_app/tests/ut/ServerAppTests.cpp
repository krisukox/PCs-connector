#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "key_management/KeyMock.hpp"
#include "server_app/ServerSession.hpp"

namespace
{
auto validErrorCode = boost::system::errc::make_error_code(boost::system::errc::success);
auto invalidErrorCode = boost::system::errc::make_error_code(boost::system::errc::bad_message);

auto validMsgSize = std::size_t{2};
auto invalidMsgSize = std::size_t{0};
} // namespace

struct ServerAppTests : public testing::Test
{
    std::shared_ptr<testing::StrictMock<mocks::KeyMock>> keyMockPtr;
    testing::StrictMock<mocks::KeyMock>* keyMockPtrRaw;

    boost::asio::io_context io_context;
    boost::asio::ip::tcp::socket socket;

    ServerAppTests()
        : keyMockPtr{std::make_shared<testing::StrictMock<mocks::KeyMock>>()}
        , keyMockPtrRaw{keyMockPtr.get()}
        , io_context{}
        , socket{io_context}
    {
    }
};

TEST_F(ServerAppTests, successfulHandleEvent_validErrorCode_validReadMsgSize)
{
    auto sut = std::make_shared<server_app::ServerSession>(std::move(socket), std::move(keyMockPtr));

    EXPECT_CALL(*keyMockPtrRaw, handleEvent(std::uint16_t{0}));
    sut->onMessage(validErrorCode, validMsgSize);
}

TEST_F(ServerAppTests, unsuccessfulHandleEvent_invalidReadMsgSize)
{
    auto sut = std::make_shared<server_app::ServerSession>(std::move(socket), std::move(keyMockPtr));

    sut->onMessage(validErrorCode, invalidMsgSize);
}

TEST_F(ServerAppTests, unsuccessfulHandleEvent_invalidErrorCode)
{
    auto sut = std::make_shared<server_app::ServerSession>(std::move(socket), std::move(keyMockPtr));

    sut->onMessage(invalidErrorCode, validMsgSize);
}

TEST_F(ServerAppTests, unsuccessfulHandleEvent_invalidErrorCode_invalidReadMsgSize)
{
    auto sut = std::make_shared<server_app::ServerSession>(std::move(socket), std::move(keyMockPtr));

    sut->onMessage(invalidErrorCode, invalidMsgSize);
}
