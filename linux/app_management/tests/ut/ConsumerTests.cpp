#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "app_management/Consumer.hpp"
//#include "connection/ReceiverMock.hpp"
#include "connection/Socket.hpp"
#include "connection/SocketMock.hpp"
#include "event_consumer/KeyboardReceiverMock.hpp"
#include "event_consumer/MouseReceiverMock.hpp"
#include "internal_types/ScreenResolution.hpp"

// namespace app_management
//{
// template class Consumer<mocks::SocketMock>;
//}

using ::testing::_;
using ::testing::MockFunction;

namespace
{
// const auto validErrorCode = boost::system::errc::make_error_code(boost::system::errc::success);
// const auto invalidErrorCode = boost::system::errc::make_error_code(boost::system::errc::bad_message);
constexpr internal_types::KeyEvent keyEvent{std::uint8_t{2}, false};
constexpr internal_types::MouseEvent mouseEvent{internal_types::MouseMoveEvent{0, 0}};

constexpr internal_types::ScreenResolution screenResolution{1080, 1920};
} // namespace

struct ConsumerTests : public testing::Test
{
    std::unique_ptr<testing::StrictMock<mocks::KeyboardReceiverMock>> keyMockPtr;
    testing::StrictMock<mocks::KeyboardReceiverMock>* keyMockPtrRaw;

    std::unique_ptr<testing::StrictMock<mocks::MouseReceiverMock>> mouseMockPtr;
    testing::StrictMock<mocks::MouseReceiverMock>* mouseMockPtrRaw;

    std::unique_ptr<testing::StrictMock<mocks::SocketMock>> socketMockPtr;
    testing::StrictMock<mocks::SocketMock>* socketMockPtrRaw;

    //    boost::asio::io_context io_context;
    //    boost::asio::ip::tcp::socket socket;

    ConsumerTests()
        : keyMockPtr{std::make_unique<testing::StrictMock<mocks::KeyboardReceiverMock>>()}
        , keyMockPtrRaw{keyMockPtr.get()}
        , mouseMockPtr{std::make_unique<testing::StrictMock<mocks::MouseReceiverMock>>()}
        , mouseMockPtrRaw{mouseMockPtr.get()}
        , socketMockPtr{std::make_unique<testing::StrictMock<mocks::SocketMock>>()}
        , socketMockPtrRaw{socketMockPtr.get()}
    //        , receiverMockPtrRaw{receiverMockPtr.get()}
    //        , io_context{}
    //        , socket{io_context}
    {
    }

    std::unique_ptr<app_management::Consumer<mocks::SocketMock>> sut;

    MockFunction<void(internal_types::ScreenResolution)> setScreenResolution;

    void createConsumerAndStart()
    {
        //                     internal_types::SetScreenResolution setScreenResolution =
        //            [](internal_types::ScreenResolution) {};

        event_consumer::IMouseReceiver::ForwardEvent forwardEvent =
            [](const internal_types::MouseChangePositionEvent&) { return; };

        //        EXPECT_CALL(*mouseMockPtrRaw, onEvent(mouseEvent));
        EXPECT_CALL(*mouseMockPtrRaw, start(_));
        sut = std::make_unique<app_management::Consumer<mocks::SocketMock>>(
            std::move(keyMockPtr),
            std::move(mouseMockPtr),
            std::move(socketMockPtr),
            setScreenResolution.AsStdFunction());

        //        EXPECT_CALL(*receiverMockPtrRaw, startReceive());
        EXPECT_CALL(*socketMockPtrRaw, receiveOnce());
        sut->start(screenResolution);
    }

    //    void expectValidHandleKeyEvent(const internal_types::KeyEvent& keyEvent)
    //    {
    //        EXPECT_CALL(*keyMockPtrRaw, onEvent(keyEvent));
    //        EXPECT_CALL(*receiverMockPtrRaw, startReceive());
    //    }

    //    void expectValidHandleMouseEvent(const internal_types::MouseEvent& mouseEvent)
    //    {
    //        EXPECT_CALL(*mouseMockPtrRaw, onEvent(mouseEvent));
    //        EXPECT_CALL(*receiverMockPtrRaw, startReceive());
    //    }

    //    void expectUnseccussfulEventReceive() { EXPECT_CALL(*receiverMockPtrRaw, startReceive()); }

    //    void callUnsuccessfulEventReceive(boost::system::error_code errorCode)
    //    {
    //        ASSERT_TRUE(receiverMockPtrRaw->unsuccessfulCallback);
    //        receiverMockPtrRaw->unsuccessfulCallback(errorCode);
    //    }

    //    void callSuccessfulEventRecive(const internal_types::Event& event)
    //    {
    //        ASSERT_TRUE(receiverMockPtrRaw->successfulCallback);
    //        receiverMockPtrRaw->successfulCallback(event);
    //    }
};

TEST_F(ConsumerTests, successfulHandleKeyEvent)
{
    testing::InSequence seq;

    createConsumerAndStart();

    EXPECT_CALL(setScreenResolution, Call(screenResolution));
    EXPECT_CALL(*socketMockPtrRaw, receive());
    EXPECT_CALL(*socketMockPtrRaw, send());

    socketMockPtrRaw->receivedOnce(screenResolution);
}

// TEST_F(ConsumerTests, successfulHandleMouseEvent)
//{
//    testing::InSequence seq;

//    createServerSessionAndStartAsyncRead();

//    expectValidHandleMouseEvent(mouseEvent);

//    callSuccessfulEventRecive(mouseEvent);
//}

// TEST_F(ConsumerTests, unsuccessfulEventReceive)
//{
//    testing::InSequence seq;

//    createServerSessionAndStartAsyncRead();

//    expectUnseccussfulEventReceive();

//    callUnsuccessfulEventReceive(validErrorCode);
//}

// TEST_F(ConsumerTests, unsuccessfulEventReceiveAndLeaveProcedure)
//{
//    testing::InSequence seq;

//    createServerSessionAndStartAsyncRead();

//    callUnsuccessfulEventReceive(invalidErrorCode);
//}
