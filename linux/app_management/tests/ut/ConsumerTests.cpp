#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "app_management/Consumer.hpp"
#include "connection/SocketMock.hpp"
#include "event_consumer/KeyboardReceiverMock.hpp"
#include "event_consumer/MouseReceiverMock.hpp"
#include "internal_types/Point.hpp"
#include "internal_types/ScreenResolution.hpp"
#include "internal_types/TransformationPoints.hpp"

using ::testing::_;
using ::testing::MockFunction;

namespace
{
constexpr internal_types::KeyEvent keyEvent{std::uint8_t{2}, false};
constexpr internal_types::MouseEvent mouseEvent{internal_types::MouseMoveEvent{2, 8}};

constexpr internal_types::ScreenResolution screenResolution{1080, 1920};

constexpr internal_types::TransformationPoints transformationPoints{
    {internal_types::Point{1, 2}, internal_types::Point{2, 3}},
    internal_types::Point{4, 5},
    internal_types::Point{6, 7}};
} // namespace

struct ConsumerTests : public testing::Test
{
    std::unique_ptr<testing::StrictMock<mocks::KeyboardReceiverMock>> keyMockPtr;
    testing::StrictMock<mocks::KeyboardReceiverMock>* keyMockPtrRaw;

    std::unique_ptr<testing::StrictMock<mocks::MouseReceiverMock>> mouseMockPtr;
    testing::StrictMock<mocks::MouseReceiverMock>* mouseMockPtrRaw;

    std::unique_ptr<testing::StrictMock<mocks::SocketMock>> socketMockPtr;
    testing::StrictMock<mocks::SocketMock>* socketMockPtrRaw;

    ConsumerTests()
        : keyMockPtr{std::make_unique<testing::StrictMock<mocks::KeyboardReceiverMock>>()}
        , keyMockPtrRaw{keyMockPtr.get()}
        , mouseMockPtr{std::make_unique<testing::StrictMock<mocks::MouseReceiverMock>>()}
        , mouseMockPtrRaw{mouseMockPtr.get()}
        , socketMockPtr{std::make_unique<testing::StrictMock<mocks::SocketMock>>()}
        , socketMockPtrRaw{socketMockPtr.get()}
    {
    }

    std::unique_ptr<app_management::Consumer<mocks::SocketMock>> sut;

    MockFunction<void(internal_types::ScreenResolution)> setScreenResolution;

    void createConsumerAndStart()
    {
        EXPECT_CALL(*mouseMockPtrRaw, start(_));
        sut = std::make_unique<app_management::Consumer<mocks::SocketMock>>(
            std::move(keyMockPtr),
            std::move(mouseMockPtr),
            std::move(socketMockPtr),
            setScreenResolution.AsStdFunction());

        EXPECT_CALL(*socketMockPtrRaw, receiveOnce());
        sut->start(screenResolution);
    }
};

TEST_F(ConsumerTests, successfulScreenResolutionExchange)
{
    testing::InSequence seq;

    createConsumerAndStart();

    EXPECT_CALL(setScreenResolution, Call(screenResolution));
    EXPECT_CALL(*socketMockPtrRaw, receive());
    EXPECT_CALL(*socketMockPtrRaw, send());

    socketMockPtrRaw->receivedOnce(screenResolution);
}

TEST_F(ConsumerTests, successfulHandleMouseEvent)
{
    testing::InSequence seq;

    createConsumerAndStart();

    EXPECT_CALL(setScreenResolution, Call(screenResolution));
    EXPECT_CALL(*socketMockPtrRaw, receive());
    EXPECT_CALL(*socketMockPtrRaw, send());

    socketMockPtrRaw->receivedOnce(screenResolution);

    EXPECT_CALL(*mouseMockPtrRaw, onEvent(mouseEvent));
    socketMockPtrRaw->received(internal_types::Event{mouseEvent});
}

TEST_F(ConsumerTests, successfulHandleKeyboardEvent)
{
    testing::InSequence seq;

    createConsumerAndStart();

    EXPECT_CALL(setScreenResolution, Call(screenResolution));
    EXPECT_CALL(*socketMockPtrRaw, receive());
    EXPECT_CALL(*socketMockPtrRaw, send());

    socketMockPtrRaw->receivedOnce(screenResolution);

    EXPECT_CALL(*keyMockPtrRaw, onEvent(keyEvent));
    socketMockPtrRaw->received(internal_types::Event{keyEvent});
}

TEST_F(ConsumerTests, successfulTransformationPointsSet)
{
    testing::InSequence seq;

    createConsumerAndStart();

    EXPECT_CALL(*mouseMockPtrRaw, setTransformationPoints(transformationPoints));
    sut->setTransformationPoints(transformationPoints);
}

TEST_F(ConsumerTests, handlerScreenResolution)
{
    testing::InSequence seq;

    createConsumerAndStart();

    EXPECT_CALL(setScreenResolution, Call(screenResolution));
    EXPECT_CALL(*socketMockPtrRaw, receive());
    EXPECT_CALL(*socketMockPtrRaw, send());

    socketMockPtrRaw->receivedOnce(screenResolution);

    socketMockPtrRaw->received(screenResolution);
}
