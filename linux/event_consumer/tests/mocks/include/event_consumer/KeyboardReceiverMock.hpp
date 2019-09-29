#pragma once
#include <gmock/gmock.h>
#include "event_consumer/IKeyboardReceiver.hpp"

namespace mocks
{
struct KeyboardReceiverMock : public event_consumer::IKeyboardReceiver
{
    MOCK_CONST_METHOD1(onEvent, void(const internal_types::KeyEvent&));
};
} // namespace mocks
