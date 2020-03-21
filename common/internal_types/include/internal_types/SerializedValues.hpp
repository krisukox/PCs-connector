#pragma once
#include <cstddef>

namespace internal_types
{
namespace serialized_values
{
inline constexpr std::byte keyEventByte{0b00000001};
inline constexpr std::byte mouseMoveByte{0b00000010};
inline constexpr std::byte mouseScrollByte{0b00000100};
inline constexpr std::byte mouseKeyByte{0b00001000};
inline constexpr std::byte mouseChangePositionByte{0b00010000};
inline constexpr std::byte screenResolutionByte{0b00100000};

inline constexpr std::byte leftButtonPressedByte{0b00000001};
inline constexpr std::byte leftButtonReleasedByte{0b00000010};
inline constexpr std::byte rightButtonPressedByte{0b00000100};
inline constexpr std::byte rightButtonReleasedByte{0b00001000};
inline constexpr std::byte middleButtonPressedByte{0b00010000};
inline constexpr std::byte middleButtonReleasedByte{0b00100000};

inline constexpr std::byte scrollForwardByte{0b11110000};
inline constexpr std::byte scrollBackwardByte{0b00001111};

inline constexpr std::byte trueValue{0b00001111};
inline constexpr std::byte falseValue{0b00000000};
} // namespace serialized_values
} // namespace internal_types
