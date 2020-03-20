#pragma once

#include "ISerializer.hpp"
#include "internal_types/ScreenResolution.hpp"

namespace internal_types
{
class Serializer : public ISerializer
{
public:
    Serializer() = default;

    ~Serializer() override = default;
    template <class T>
    Buffer encode(const T& event) const
    {
        return encode(event);
    }
    Buffer encode(const Event&) const override;

private:
    Buffer encode(const ScreenResolution&) const;
    Buffer encode(const MouseEvent&) const;
    Buffer encode(const KeyEvent&) const;
    Buffer encode(const MouseMoveEvent&) const;
    Buffer encode(const MouseScrollEvent&) const;
    Buffer encode(const MouseKeyEvent&) const;
    Buffer encode(const MouseChangePositionEvent&) const;
};
} // namespace internal_types
