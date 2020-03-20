#pragma once

#include <X11/Xlib.h>
#include <stdexcept>
#include <type_traits>
#include <unordered_map>
#include <variant>
#include "IDeserializer.hpp"
#include "KeyEvent.hpp"
#include "MouseEvent.hpp"
#include "ScreenResolution.hpp"

namespace internal_types
{
class Deserializer
{
public:
    Deserializer(Display*);

    ~Deserializer() = default;

    template <class T>
    T decode(const internal_types::Buffer& buffer);

private:
    internal_types::Event decodeEvent(const internal_types::Buffer&) const;
    internal_types::ScreenResolution decodeScreenResolution(const internal_types::Buffer&) const;

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
