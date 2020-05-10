#pragma once
#include <gmock/gmock.h>
#include "event_consumer/IMouseReceiver.hpp"

namespace mocks
{
struct MouseReceiverMock : public event_consumer::IMouseReceiver
{
    MOCK_METHOD(void, start, (ForwardEvent));
    MOCK_METHOD(void, onEvent, (const internal_types::MouseEvent&));
    MOCK_METHOD(void, setTransformationPoints, (const internal_types::TransformationPoints&));
};
} // namespace mocks
