#pragma once
#include <gmock/gmock.h>
#include "event_consumer/IKeyboardReceiver.hpp"

namespace mocks
{
struct KeyboardReceiverMock : public event_consumer::IKeyboardReceiver
{
    MOCK_METHOD(void, onEvent, (const internal_types::KeyEvent&), (const));
};
} // namespace mocks
