#pragma once
#include <gmock/gmock.h>
#include "event_consumer/IMouseReceiver.hpp"

namespace mocks
{
struct MouseReceiverMock : public event_consumer::IMouseReceiver
{
    MOCK_METHOD1(onEvent, void(const internal_types::MouseEvent&));
};
} // namespace mocks
