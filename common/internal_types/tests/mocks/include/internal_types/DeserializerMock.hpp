#pragma once
#include "internal_types/IDeserializer.hpp"
#include <gmock/gmock.h>

namespace mocks {
struct DeserilizerMock : public internal_types::IDeserializer {
  MOCK_CONST_METHOD1(
      decode,
      std::variant<internal_types::KeyEvent, internal_types::MouseEvent>(
          const internal_types::Buffer &));
};
} // namespace mocks
