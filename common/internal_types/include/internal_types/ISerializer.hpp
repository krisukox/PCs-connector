#pragma once

#include <variant>
#include "CommonTypes.hpp"
#include "MouseEvent.hpp"

namespace internal_types
{
class ISerializer
{
public:
    virtual ~ISerializer() = default;

    virtual Buffer encode(const Event&) const = 0;
};
} // namespace internal_types
