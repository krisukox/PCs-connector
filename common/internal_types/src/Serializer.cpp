#include "internal_types/Serializer.hpp"
#include <array>
#include <cstddef>
#include <variant>

namespace
{
constexpr std::byte keyEventByte{0b00000001};
constexpr std::byte mouseMoveByte{0b00000010};
constexpr std::byte mouseScrollByte{0b00000100};
constexpr std::byte mouseKeyByte{0b00001000};
constexpr std::byte mouseChangePositionByte{0b00010000};
constexpr std::byte screenResolutionByte{0b00100000};

constexpr std::byte leftButtonPressedByte{0b00000001};
constexpr std::byte leftButtonReleasedByte{0b00000010};
constexpr std::byte rightButtonPressedByte{0b00000100};
constexpr std::byte rightButtonReleasedByte{0b00001000};
constexpr std::byte middleButtonPressedByte{0b00010000};
constexpr std::byte middleButtonReleasedByte{0b00100000};

constexpr std::byte scrollForwardByte{0b11110000};
constexpr std::byte scrollBackwardByte{0b00001111};

constexpr std::byte True{0b00001111};
constexpr std::byte False{0b00000000};

std::array<std::byte, 2> toBytes(const short& val)
{
    return {static_cast<std::byte>(val >> 8), static_cast<std::byte>(val)};
}

std::byte toByte(const bool isPressed)
{
    if (isPressed)
    {
        return True;
    }
    return False;
}
} // namespace

namespace internal_types
{
Buffer Serializer::encode(const Event& event) const
{
    return std::visit([this](const auto& event) -> Buffer { return encode(event); }, event);
}

Buffer Serializer::encode(const KeyEvent& keyEvent) const
{
    return {keyEventByte, std::byte(keyEvent.keyCode), toByte(keyEvent.isPressed)};
}

Buffer Serializer::encode(const MouseEvent& mouseEvent) const
{
    return std::visit([this](const auto& event) -> Buffer { return encode(event); }, mouseEvent);
}

Buffer Serializer::encode(const MouseMoveEvent& mouseMoveEvent) const
{
    auto [xPart1, xPart2] = toBytes(mouseMoveEvent.deltaX);
    auto [yPart1, yPart2] = toBytes(mouseMoveEvent.deltaY);

    return {mouseMoveByte, xPart1, xPart2, yPart1, yPart2};
}

Buffer Serializer::encode(const MouseScrollEvent& mouseScrollEvent) const
{
    if (mouseScrollEvent == MouseScrollEvent::Forward)
    {
        return {mouseScrollByte, scrollForwardByte};
    }
    if (mouseScrollEvent == MouseScrollEvent::Backward)
    {
        return {mouseScrollByte, scrollBackwardByte};
    }
    throw std::runtime_error("Unexpected MouseScrollEvent value");
}

Buffer Serializer::encode(const MouseKeyEvent& mouseKeyEvent) const
{
    switch (mouseKeyEvent)
    {
        case MouseKeyEvent::LeftButtonPressed:
            return {mouseKeyByte, leftButtonPressedByte};
        case MouseKeyEvent::LeftButtonUnpressed:
            return {mouseKeyByte, leftButtonReleasedByte};
        case MouseKeyEvent::RightButtonPressed:
            return {mouseKeyByte, rightButtonPressedByte};
        case MouseKeyEvent::RightButtonUnpressed:
            return {mouseKeyByte, rightButtonReleasedByte};
        case MouseKeyEvent::MiddleButtonPressed:
            return {mouseKeyByte, middleButtonPressedByte};
        case MouseKeyEvent::MiddleButtonUnpressed:
            return {mouseKeyByte, middleButtonReleasedByte};
    }
    throw std::runtime_error("Unexpected MouseKeyEvent value");
}

Buffer Serializer::encode(const MouseChangePositionEvent& event) const
{
    auto [xPart1, xPart2] = toBytes(event.x);
    auto [yPart1, yPart2] = toBytes(event.y);

    return {mouseChangePositionByte, xPart1, xPart2, yPart1, yPart2};
}

Buffer Serializer::encode(const ScreenResolution& screenResolution) const
{
    auto [widthPart1, widthPart2] = toBytes(screenResolution.width);
    auto [heightPart1, heightPart2] = toBytes(screenResolution.height);

    return {screenResolutionByte, widthPart1, widthPart2, heightPart1, heightPart2};
}
} // namespace internal_types
