#include "internal_types/DeserializerWin.hpp"
#include <iostream>

namespace
{
short toShort(const std::byte lv, const std::byte rv)
{
    return static_cast<short>((std::to_integer<uint8_t>(lv) << 8) + std::to_integer<uint8_t>(rv));
}
} // namespace

namespace internal_types
{
Deserializer::Deserializer() {}

internal_types::Event Deserializer::decode(const internal_types::Buffer& buffer) const
{
    if (buffer[0] != std::byte{0b00010000})
    {
        return MouseChangePositionEvent{};
    }

    return MouseChangePositionEvent{toShort(buffer[1], buffer[2]), toShort(buffer[3], buffer[4])};
}
} // namespace internal_types
