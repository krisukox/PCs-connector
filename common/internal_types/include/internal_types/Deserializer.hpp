#pragma once

#include <X11/Xlib.h>
#include <unordered_map>
#include <variant>
#include "IDeserializer.hpp"
#include "KeyEvent.hpp"
#include "MouseEvent.hpp"

namespace internal_types
{
class Deserializer : public IDeserializer
{
public:
    Deserializer(Display*);

    ~Deserializer() override = default;
    internal_types::Event decode(const internal_types::Buffer&) const override;

private:
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
