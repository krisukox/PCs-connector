#include "internal_types/Serializer.hpp"
#include <array>
#include <cstddef>
#include <utility>

namespace
{
std::array<std::byte, 2> toBytes(const short& val)
{
    return {static_cast<std::byte>(val >> 8), static_cast<std::byte>(val)};
}
} // namespace

namespace internal_types
{
Buffer Serializer::encode(const Event& event) const
{
    return std::visit([this](const auto& event) -> Buffer { return encode(event); }, event);
}

Buffer Serializer::encode(const KeyEvent&) const
{
    return {};
}

Buffer Serializer::encode(const MouseEvent& mouseEvent) const
{
    return std::visit([this](const auto& event) -> Buffer { return encode(event); }, mouseEvent);
}

Buffer Serializer::encode(const MouseMoveEvent& mouseMoveEvent) const
{
    std::byte mouseMoveByte{0b11111101};

    auto [xPart1, xPart2] = toBytes(mouseMoveEvent.deltaX);
    auto [yPart1, yPart2] = toBytes(mouseMoveEvent.deltaY);

    return {mouseMoveByte, xPart1, xPart2, yPart1, yPart2};
}

Buffer Serializer::encode(const MouseScrollEvent&) const
{
    return {};
}

Buffer Serializer::encode(const MouseKeyEvent&) const
{
    return {};
}
} // namespace internal_types
