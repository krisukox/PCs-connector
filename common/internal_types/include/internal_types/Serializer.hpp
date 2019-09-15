#pragma once

#include "ISerializer.hpp"

namespace internal_types
{
class Serializer : public ISerializer
{
public:
    Serializer() = default;

    ~Serializer() override = default;
    Buffer encode(const Event&) const override;

private:
    Buffer encode(const MouseEvent&) const;
    Buffer encode(const KeyEvent&) const;
    Buffer encode(const MouseMoveEvent&) const;
    Buffer encode(const MouseScrollEvent&) const;
    Buffer encode(const MouseKeyEvent&) const;
};
} // namespace internal_types
