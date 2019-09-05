#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "internal_types/DeserializerMock.hpp"
#include "key_management/KeyMock.hpp"
#include "mouse_management/FakeMouse.hpp"
#include "mouse_management/MouseMock.hpp"
#include "server_app/ReceiverMock.hpp"
#include "server_app/ServerAppTypes.hpp"
#include "server_app/ServerSession.hpp"

namespace
{
const auto validErrorCode = boost::system::errc::make_error_code(boost::system::errc::success);
const auto invalidErrorCode = boost::system::errc::make_error_code(boost::system::errc::bad_message);

constexpr std::size_t validMsgSize{2};
constexpr std::size_t invalidMsgSize{0};

constexpr std::byte keyId{34};

constexpr internal_types::KeyEvent keyEvent{KeyCode{2}, false};
constexpr internal_types::MouseEvent mouseEvent{internal_types::MouseMoveEvent{0, 0}};

constexpr server_app::Buffer buffer{keyId, std::byte{1}};
} // namespace

struct ServerAppTests : public testing::Test
{
    std::shared_ptr<testing::StrictMock<mocks::KeyMock>> keyMockPtr;
    testing::StrictMock<mocks::KeyMock>* keyMockPtrRaw;

    std::shared_ptr<testing::StrictMock<mocks::MouseMock>> mouseMockPtr;
    testing::StrictMock<mocks::MouseMock>* mouseMockPtrRaw;

    std::shared_ptr<testing::StrictMock<mocks::ReceiverMock>> receiverMockPtr;
    testing::StrictMock<mocks::ReceiverMock>* receiverMockPtrRaw;

    std::unique_ptr<testing::StrictMock<mocks::DeserilizerMock>> deserializerMockPtr;
    testing::StrictMock<mocks::DeserilizerMock>* deserializerMockPtrRaw;

    boost::asio::io_context io_context;
    boost::asio::ip::tcp::socket socket;

    ServerAppTests()
        : keyMockPtr{std::make_shared<testing::StrictMock<mocks::KeyMock>>()}
        , keyMockPtrRaw{keyMockPtr.get()}
        , mouseMockPtr{std::make_shared<testing::StrictMock<mocks::MouseMock>>()}
        , mouseMockPtrRaw{mouseMockPtr.get()}
        , receiverMockPtr{std::make_shared<testing::StrictMock<mocks::ReceiverMock>>()}
        , receiverMockPtrRaw{receiverMockPtr.get()}
        , deserializerMockPtr{std::make_unique<testing::StrictMock<mocks::DeserilizerMock>>()}
        , deserializerMockPtrRaw{deserializerMockPtr.get()}
        , io_context{}
        , socket{io_context}
    {
    }

    std::unique_ptr<server_app::ServerSession> sut;

    void createServerSessionAndStartAsyncRead()
    {
        sut = std::make_unique<server_app::ServerSession>(
            std::move(socket),
            std::move(keyMockPtr),
            std::move(mouseMockPtr),
            std::move(receiverMockPtr),
            std::move(deserializerMockPtr));

        EXPECT_CALL(*receiverMockPtrRaw, startAsyncRead());
        sut->start();
    }

    void expectValidHandleKeyEvent()
    {
        EXPECT_CALL(*deserializerMockPtrRaw, decode(buffer)).Times(1).WillOnce(testing::Return(keyEvent));
        EXPECT_CALL(*keyMockPtrRaw, onEvent(keyEvent));
        EXPECT_CALL(*receiverMockPtrRaw, startAsyncRead());
    }

    void expectValidHandleMouseEvent()
    {
        EXPECT_CALL(*deserializerMockPtrRaw, decode(buffer)).Times(1).WillOnce(testing::Return(mouseEvent));
        EXPECT_CALL(*mouseMockPtrRaw, onEvent(mouseEvent));
        EXPECT_CALL(*receiverMockPtrRaw, startAsyncRead());
    }

    void deserializerThrowException()
    {
        EXPECT_CALL(*deserializerMockPtrRaw, decode(buffer))
            .Times(1)
            .WillOnce(testing::Throw(std::out_of_range("Key not supported")));
        EXPECT_CALL(*receiverMockPtrRaw, startAsyncRead());
    }

    void expectValidCatchOfException() {}

    void callKeyEvent(boost::system::error_code errorCode, std::size_t receivedMsgSize)
    {
        receiverMockPtrRaw->setReturnBuffer(buffer);
        ASSERT_TRUE(receiverMockPtrRaw->callHandler);
        receiverMockPtrRaw->callHandler(errorCode, receivedMsgSize);
    }
};

TEST_F(ServerAppTests, successfulHandleKeyEvent)
{
    testing::InSequence seq;

    createServerSessionAndStartAsyncRead();

    expectValidHandleKeyEvent();

    callKeyEvent(validErrorCode, validMsgSize);
}

TEST_F(ServerAppTests, successfulHandleMouseEvent)
{
    testing::InSequence seq;

    createServerSessionAndStartAsyncRead();

    expectValidHandleMouseEvent();

    callKeyEvent(validErrorCode, validMsgSize);
}

TEST_F(ServerAppTests, successfulExceptionCatch)
{
    testing::InSequence seq;

    createServerSessionAndStartAsyncRead();

    deserializerThrowException();

    EXPECT_NO_THROW(callKeyEvent(validErrorCode, validMsgSize));
}

TEST_F(ServerAppTests, unsuccessfulHandleEvent_invalidErrorCode_validReadMsgSize)
{
    testing::InSequence seq;

    createServerSessionAndStartAsyncRead();

    callKeyEvent(invalidErrorCode, validMsgSize);
}

TEST_F(ServerAppTests, unsuccessfulHandleEvent_validErrorCode_invalidReadMsgSize)
{
    testing::InSequence seq;

    createServerSessionAndStartAsyncRead();

    EXPECT_CALL(*receiverMockPtrRaw, startAsyncRead());

    callKeyEvent(validErrorCode, invalidMsgSize);
}

TEST_F(ServerAppTests, unsuccessfulHandleEvent_invalidErrorCode_invalidReadMsgSize)
{
    testing::InSequence seq;

    createServerSessionAndStartAsyncRead();

    callKeyEvent(invalidErrorCode, invalidMsgSize);
}
