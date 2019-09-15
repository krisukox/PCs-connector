#pragma once

#include <array>
#include <cstddef>
#include <variant>
#include "KeyEvent.hpp"
#include "MouseEvent.hpp"

namespace internal_types
{
using Buffer = std::array<std::byte, 5>;
using Event = std::variant<KeyEvent, MouseEvent>;
} // namespace internal_types
