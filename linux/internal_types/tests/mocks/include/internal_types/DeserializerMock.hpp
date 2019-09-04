#pragma once
#include <gmock/gmock.h>
#include "internal_types/IDeserializer.hpp"

namespace mocks
{
struct DeserilizerMock : public internal_types::IDeserializer
{
    MOCK_CONST_METHOD1(
        decode,
        std::variant<internal_types::KeyEvent, internal_types::MouseEvent>(const server_app::Buffer&));
};
} // namespace mocks
