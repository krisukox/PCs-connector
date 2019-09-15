#pragma once
#include <gmock/gmock.h>
#include "mouse_management/IMouse.hpp"

namespace mocks
{
struct MouseMock : public mouse_management::IMouse
{
    MOCK_CONST_METHOD1(onEvent, void(const internal_types::MouseEvent&));
};
} // namespace mocks
