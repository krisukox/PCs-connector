#pragma once
#include <cstddef>

namespace internal_types
{
namespace serialized_values
{
inline constexpr std::byte keyEvent{0b00000001};
inline constexpr std::byte mouseMove{0b00000010};
inline constexpr std::byte mouseScroll{0b00000100};
inline constexpr std::byte mouseKey{0b00001000};
inline constexpr std::byte mouseChangePosition{0b00010000};
inline constexpr std::byte screenResolution{0b00100000};

inline constexpr std::byte leftButtonPressed{0b00000001};
inline constexpr std::byte leftButtonReleased{0b00000010};
inline constexpr std::byte rightButtonPressed{0b00000100};
inline constexpr std::byte rightButtonReleased{0b00001000};
inline constexpr std::byte middleButtonPressed{0b00010000};
inline constexpr std::byte middleButtonReleased{0b00100000};

inline constexpr std::byte scrollForward{0b11110000};
inline constexpr std::byte scrollBackward{0b00001111};

inline constexpr std::byte trueValue{0b00001111};
inline constexpr std::byte falseValue{0b00000000};
} // namespace serialized_values
} // namespace internal_types
