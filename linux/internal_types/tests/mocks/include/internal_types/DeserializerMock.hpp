#pragma once
#include <gmock/gmock.h>
#include "internal_types/IDeserializer.hpp"

namespace mocks
{
struct DeserilizerMock : public internal_types::IDeserializer
{
    MOCK_CONST_METHOD1(decode, internal_types::KeyEvent(const server_app::Buffer&));
};
} // namespace mocks
