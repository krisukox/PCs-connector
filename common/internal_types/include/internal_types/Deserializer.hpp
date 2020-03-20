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
    T foo()
    {
    }
    template <>
    int foo<int>()
    {
    }

    template <class T>
    T decode(const internal_types::Buffer& buffer)
    {
        if (std::is_same<T, internal_types::Event>::value)
        {
            return decodeEvent(buffer);
        }
        if (std::is_same<T, internal_types::ScreenResolution>::value)
        {
            return decodeScreenResolution(buffer);
        }
        //        throw std::runtime_error<>;
    }

    template <>
    internal_types::Event decode<internal_types::Event>(const internal_types::Buffer&)
    {
        return {};
    }

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
