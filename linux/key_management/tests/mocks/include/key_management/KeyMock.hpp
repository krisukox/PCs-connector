#pragma once
#include <gmock/gmock.h>
#include "key_management/IKey.hpp"

namespace mocks
{
struct KeyMock : public key_management::IKey
{
    MOCK_CONST_METHOD1(handleEvent, void(std::uint16_t));
};
} // namespace mocks
