#include "Deserializer.hpp"
#include <iostream>

namespace
{
constexpr std::byte mouseChangePositionByte{0b00010000};
constexpr std::byte screenResolutionByte{0b00100000};

short toShort(const std::byte lv, const std::byte rv)
{
    return static_cast<short>((std::to_integer<uint8_t>(lv) << 8) + std::to_integer<uint8_t>(rv));
}

std::uint16_t toUInt(const std::byte lv, const std::byte rv)
{
    return (std::to_integer<uint8_t>(lv) << 8) + std::to_integer<uint8_t>(rv);
}
} // namespace

namespace internal_types
{
Deserializer::Deserializer() {}

std::optional<internal_types::DecodedType> Deserializer::decode(const internal_types::Buffer& buffer) const
{
    if (buffer[0] == mouseChangePositionByte)
    {
        return decodeEvent(buffer);
    }
    if (buffer[0] == screenResolutionByte)
    {
        return decodeScreenResolution(buffer);
    }
    return std::nullopt;
}

internal_types::Event Deserializer::decodeEvent(const internal_types::Buffer& buffer) const
{
    return MouseChangePositionEvent{toShort(buffer[1], buffer[2]), toShort(buffer[3], buffer[4])};
}

internal_types::ScreenResolution Deserializer::decodeScreenResolution(const internal_types::Buffer& buffer) const
{
    return {toUInt(buffer[1], buffer[2]), toUInt(buffer[3], buffer[4])};
}
} // namespace internal_types
