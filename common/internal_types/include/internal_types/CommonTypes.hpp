#pragma once

#include <array>
#include <cstddef>
#include <variant>
#include "KeyEvent.hpp"
#include "MouseEvent.hpp"
#include "ScreenResolution.hpp"

namespace internal_types
{
using Buffer = std::array<std::byte, 5>;
using Event = std::variant<KeyEvent, MouseEvent>;
using DecodedType = std::variant<Event, ScreenResolution>;
} // namespace internal_types
