#pragma once

#include "internal_types/CommonTypes.hpp"
#include "internal_types/ScreenResolution.hpp"

namespace internal_types
{
class Serializer
{
public:
    Serializer() = default;

    ~Serializer() = default;
    template <class T>
    Buffer encode(const T& event) const
    {
        return encode(event);
    }

private:
    Buffer encode(const Event&) const;
    Buffer encode(const ScreenResolution&) const;
    Buffer encode(const MouseEvent&) const;
    Buffer encode(const KeyEvent&) const;
    Buffer encode(const MouseMoveEvent&) const;
    Buffer encode(const MouseScrollEvent&) const;
    Buffer encode(const MouseKeyEvent&) const;
    Buffer encode(const MouseChangePositionEvent&) const;
};
} // namespace internal_types
