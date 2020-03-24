#include "internal_types/Serializer.hpp"
#include <array>
#include <cstddef>
#include <variant>
#include "internal_types/SerializedValues.hpp"

namespace internal_types
{
namespace
{
std::array<std::byte, 2> toBytes(const short& val)
{
    return {static_cast<std::byte>(val >> 8), static_cast<std::byte>(val)};
}

std::byte toByte(const bool isPressed)
{
    if (isPressed)
    {
        return serialized_values::trueValue;
    }
    return serialized_values::falseValue;
}
} // namespace

Buffer Serializer::encode(const Event& event) const
{
    return std::visit([this](const auto& event) -> Buffer { return encode(event); }, event);
}

Buffer Serializer::encode(const KeyEvent& keyEvent) const
{
    return {serialized_values::keyEvent, std::byte(keyEvent.keyCode), toByte(keyEvent.isPressed)};
}

Buffer Serializer::encode(const MouseEvent& mouseEvent) const
{
    return std::visit([this](const auto& event) -> Buffer { return encode(event); }, mouseEvent);
}

Buffer Serializer::encode(const MouseMoveEvent& mouseMoveEvent) const
{
    auto [xPart1, xPart2] = toBytes(mouseMoveEvent.deltaX);
    auto [yPart1, yPart2] = toBytes(mouseMoveEvent.deltaY);

    return {serialized_values::mouseMove, xPart1, xPart2, yPart1, yPart2};
}

Buffer Serializer::encode(const MouseScrollEvent& mouseScrollEvent) const
{
    if (mouseScrollEvent == MouseScrollEvent::Forward)
    {
        return {serialized_values::mouseScroll, serialized_values::scrollForward};
    }
    if (mouseScrollEvent == MouseScrollEvent::Backward)
    {
        return {serialized_values::mouseScroll, serialized_values::scrollBackward};
    }
    throw std::runtime_error("Unexpected MouseScrollEvent value");
}

Buffer Serializer::encode(const MouseKeyEvent& mouseKeyEvent) const
{
    switch (mouseKeyEvent)
    {
        case MouseKeyEvent::LeftButtonPressed:
            return {serialized_values::mouseKey, serialized_values::leftButtonPressed};
        case MouseKeyEvent::LeftButtonUnpressed:
            return {serialized_values::mouseKey, serialized_values::leftButtonReleased};
        case MouseKeyEvent::RightButtonPressed:
            return {serialized_values::mouseKey, serialized_values::rightButtonPressed};
        case MouseKeyEvent::RightButtonUnpressed:
            return {serialized_values::mouseKey, serialized_values::rightButtonReleased};
        case MouseKeyEvent::MiddleButtonPressed:
            return {serialized_values::mouseKey, serialized_values::middleButtonPressed};
        case MouseKeyEvent::MiddleButtonUnpressed:
            return {serialized_values::mouseKey, serialized_values::middleButtonReleased};
    }
    throw std::runtime_error("Unexpected MouseKeyEvent value");
}

Buffer Serializer::encode(const MouseChangePositionEvent& event) const
{
    auto [xPart1, xPart2] = toBytes(event.x);
    auto [yPart1, yPart2] = toBytes(event.y);

    return {serialized_values::mouseChangePosition, xPart1, xPart2, yPart1, yPart2};
}

Buffer Serializer::encode(const ScreenResolution& screenResolution) const
{
    auto [widthPart1, widthPart2] = toBytes(screenResolution.width);
    auto [heightPart1, heightPart2] = toBytes(screenResolution.height);

    return {serialized_values::screenResolution, widthPart1, widthPart2, heightPart1, heightPart2};
}
} // namespace internal_types
