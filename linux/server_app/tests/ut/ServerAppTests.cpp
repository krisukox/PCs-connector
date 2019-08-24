#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "key_management/KeyMock.hpp"
#include "server_app/ReceiverMock.hpp"
#include "server_app/ServerSession.hpp"

namespace
{
const auto validErrorCode = boost::system::errc::make_error_code(boost::system::errc::success);
const auto invalidErrorCode = boost::system::errc::make_error_code(boost::system::errc::bad_message);

constexpr std::size_t validMsgSize{2};
constexpr std::size_t invalidMsgSize{0};

constexpr std::byte testKeyId{34};

constexpr std::byte dummyKeyId{0};
constexpr std::byte dummyKeyState{0};
} // namespace

struct ServerAppTests : public testing::Test
{
    std::shared_ptr<testing::StrictMock<mocks::KeyMock>> keyMockPtr;
    testing::StrictMock<mocks::KeyMock>* keyMockPtrRaw;

    std::shared_ptr<testing::StrictMock<mocks::ReceiverMock>> receiverMockPtr;
    testing::StrictMock<mocks::ReceiverMock>* receiverMockPtrRaw;

    boost::asio::io_context io_context;
    boost::asio::ip::tcp::socket socket;

    ServerAppTests()
        : keyMockPtr{std::make_shared<testing::StrictMock<mocks::KeyMock>>()}
        , keyMockPtrRaw{keyMockPtr.get()}
        , receiverMockPtr{std::make_shared<testing::StrictMock<mocks::ReceiverMock>>()}
        , receiverMockPtrRaw{receiverMockPtr.get()}
        , io_context{}
        , socket{io_context}
    {
    }

    std::unique_ptr<server_app::ServerSession> sut;

    void createServerSessionAndStartAsyncRead()
    {
        sut = std::make_unique<server_app::ServerSession>(
            std::move(socket), std::move(keyMockPtr), std::move(receiverMockPtr));

        EXPECT_CALL(*receiverMockPtrRaw, startAsyncRead());
        sut->start();
    }

    void expectValidHandleKeyEvent(std::byte keyId, bool isPressed)
    {
        EXPECT_CALL(*keyMockPtrRaw, handleEvent(keyId, isPressed));
        EXPECT_CALL(*receiverMockPtrRaw, startAsyncRead());
    }

    void callKeyEvent(const server_app::Buffer buffer, boost::system::error_code errorCode, std::size_t receivedMsgSize)
    {
        receiverMockPtrRaw->setReturnBuffer(buffer);
        ASSERT_TRUE(receiverMockPtrRaw->callHandler);
        receiverMockPtrRaw->callHandler(errorCode, receivedMsgSize);
    }
};

TEST_F(ServerAppTests, successfulHandleEvent_keyUp)
{
    testing::InSequence seq;

    bool isPressed = false;
    std::byte keyState = std::byte{0};

    createServerSessionAndStartAsyncRead();

    expectValidHandleKeyEvent(testKeyId, isPressed);

    callKeyEvent({testKeyId, keyState}, validErrorCode, validMsgSize);
}

TEST_F(ServerAppTests, successfulHandleEvent_keyDown)
{
    testing::InSequence seq;

    bool isPressed = true;
    std::byte keyState = std::byte{1};

    createServerSessionAndStartAsyncRead();

    expectValidHandleKeyEvent(testKeyId, isPressed);

    callKeyEvent({testKeyId, keyState}, validErrorCode, validMsgSize);
}

TEST_F(ServerAppTests, unsuccessfulHandleEvent_invalidErrorCode_validReadMsgSize)
{
    testing::InSequence seq;

    createServerSessionAndStartAsyncRead();

    callKeyEvent({dummyKeyId, dummyKeyState}, invalidErrorCode, validMsgSize);
}

TEST_F(ServerAppTests, unsuccessfulHandleEvent_validErrorCode_invalidReadMsgSize)
{
    testing::InSequence seq;

    createServerSessionAndStartAsyncRead();

    EXPECT_CALL(*receiverMockPtrRaw, startAsyncRead());

    callKeyEvent({dummyKeyId, dummyKeyState}, validErrorCode, invalidMsgSize);
}

TEST_F(ServerAppTests, unsuccessfulHandleEvent_invalidErrorCode_invalidReadMsgSize)
{
    testing::InSequence seq;

    createServerSessionAndStartAsyncRead();

    callKeyEvent({dummyKeyId, dummyKeyState}, invalidErrorCode, invalidMsgSize);
}
