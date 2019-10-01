#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "app_management/Consumer.hpp"
#include "connection/ReceiverMock.hpp"
#include "event_consumer/KeyboardReceiverMock.hpp"
#include "event_consumer/MouseReceiverMock.hpp"

namespace
{
const auto validErrorCode = boost::system::errc::make_error_code(boost::system::errc::success);
const auto invalidErrorCode = boost::system::errc::make_error_code(boost::system::errc::bad_message);

constexpr internal_types::KeyEvent keyEvent{std::uint8_t{2}, false};
constexpr internal_types::MouseEvent mouseEvent{internal_types::MouseMoveEvent{0, 0}};
} // namespace

struct ConsumerTests : public testing::Test
{
    std::shared_ptr<testing::StrictMock<mocks::KeyboardReceiverMock>> keyMockPtr;
    testing::StrictMock<mocks::KeyboardReceiverMock>* keyMockPtrRaw;

    std::shared_ptr<testing::StrictMock<mocks::MouseReceiverMock>> mouseMockPtr;
    testing::StrictMock<mocks::MouseReceiverMock>* mouseMockPtrRaw;

    std::shared_ptr<testing::StrictMock<mocks::ReceiverMock>> receiverMockPtr;
    testing::StrictMock<mocks::ReceiverMock>* receiverMockPtrRaw;

    boost::asio::io_context io_context;
    boost::asio::ip::tcp::socket socket;

    ConsumerTests()
        : keyMockPtr{std::make_shared<testing::StrictMock<mocks::KeyboardReceiverMock>>()}
        , keyMockPtrRaw{keyMockPtr.get()}
        , mouseMockPtr{std::make_shared<testing::StrictMock<mocks::MouseReceiverMock>>()}
        , mouseMockPtrRaw{mouseMockPtr.get()}
        , receiverMockPtr{std::make_shared<testing::StrictMock<mocks::ReceiverMock>>()}
        , receiverMockPtrRaw{receiverMockPtr.get()}
        , io_context{}
        , socket{io_context}
    {
    }

    std::unique_ptr<app_management::Consumer> sut;

    void createServerSessionAndStartAsyncRead()
    {
        sut = std::make_unique<app_management::Consumer>(
            std::move(keyMockPtr), std::move(mouseMockPtr), std::move(receiverMockPtr));

        EXPECT_CALL(*receiverMockPtrRaw, startReceive());
        sut->start();
    }

    void expectValidHandleKeyEvent(const internal_types::KeyEvent& keyEvent)
    {
        EXPECT_CALL(*keyMockPtrRaw, onEvent(keyEvent));
        EXPECT_CALL(*receiverMockPtrRaw, startReceive());
    }

    void expectValidHandleMouseEvent(const internal_types::MouseEvent& mouseEvent)
    {
        EXPECT_CALL(*mouseMockPtrRaw, onEvent(mouseEvent));
        EXPECT_CALL(*receiverMockPtrRaw, startReceive());
    }

    void expectUnseccussfulEventReceive() { EXPECT_CALL(*receiverMockPtrRaw, startReceive()); }

    void callUnsuccessfulEventReceive(boost::system::error_code errorCode)
    {
        ASSERT_TRUE(receiverMockPtrRaw->unsuccessfulCallback);
        receiverMockPtrRaw->unsuccessfulCallback(errorCode);
    }

    void callSuccessfulEventRecive(const internal_types::Event& event)
    {
        ASSERT_TRUE(receiverMockPtrRaw->successfulCallback);
        receiverMockPtrRaw->successfulCallback(event);
    }
};

TEST_F(ConsumerTests, successfulHandleKeyEvent)
{
    testing::InSequence seq;

    createServerSessionAndStartAsyncRead();

    expectValidHandleKeyEvent(keyEvent);

    callSuccessfulEventRecive(keyEvent);
}

TEST_F(ConsumerTests, successfulHandleMouseEvent)
{
    testing::InSequence seq;

    createServerSessionAndStartAsyncRead();

    expectValidHandleMouseEvent(mouseEvent);

    callSuccessfulEventRecive(mouseEvent);
}

TEST_F(ConsumerTests, unsuccessfulEventReceive)
{
    testing::InSequence seq;

    createServerSessionAndStartAsyncRead();

    expectUnseccussfulEventReceive();

    callUnsuccessfulEventReceive(validErrorCode);
}

TEST_F(ConsumerTests, unsuccessfulEventReceiveAndLeaveProcedure)
{
    testing::InSequence seq;

    createServerSessionAndStartAsyncRead();

    callUnsuccessfulEventReceive(invalidErrorCode);
}
