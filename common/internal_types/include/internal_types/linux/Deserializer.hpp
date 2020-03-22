#pragma once

#include <X11/Xlib.h>
#include <optional>
#include <stdexcept>
#include <type_traits>
#include <unordered_map>
#include <variant>
//#include "IDeserializer.hpp"
#include "internal_types/CommonTypes.hpp"
#include "internal_types/KeyEvent.hpp"
#include "internal_types/MouseEvent.hpp"
#include "internal_types/ScreenResolution.hpp"

namespace internal_types
{
class Deserializer
{
public:
    Deserializer(Display*);

    ~Deserializer() = default;

    std::optional<DecodedType> decode(const internal_types::Buffer& buffer) const;

private:
    DecodedType decodeInternal(const internal_types::Buffer& buffer) const;

    internal_types::ScreenResolution decodeScreenResolution(const internal_types::Buffer&) const;
    std::optional<internal_types::Event> decodeEvent(const internal_types::Buffer&) const;

    KeyEvent decodeKeyEvent(const internal_types::Buffer& buffer) const;
    KeyCode decodeKeyCode(const std::byte&) const;
    bool decodeKeyState(const std::byte&) const;
    MouseMoveEvent decodeMouseMoveEvent(const internal_types::Buffer&) const;
    MouseScrollEvent decodeMouseScrollEvent(const internal_types::Buffer&) const;
    MouseChangePositionEvent decodeMouseChangePositionEvent(const internal_types::Buffer&) const;
    MouseKeyEvent decodeMouseKeyEvent(const internal_types::Buffer&) const;

    Display* display;
    const std::unordered_map<std::byte, KeyCode> translationTabel;
};
} // namespace internal_types
