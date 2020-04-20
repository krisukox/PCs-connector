#pragma once
#include <gmock/gmock.h>
#include "event_consumer/IMouseReceiver.hpp"

namespace mocks
{
struct MouseReceiverMock : public event_consumer::IMouseReceiver
{
    MOCK_METHOD1(start, void(ForwardEvent));
    MOCK_METHOD1(onEvent, void(const internal_types::MouseEvent&));
    MOCK_METHOD1(setTransformationPoints, void(const internal_types::TransformationPoints&));
};
} // namespace mocks
