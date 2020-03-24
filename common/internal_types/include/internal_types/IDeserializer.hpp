#pragma once

#include <optional>
#include <variant>
#include "KeyEvent.hpp"
#include "MouseEvent.hpp"
#include "internal_types/CommonTypes.hpp"

namespace internal_types
{
class IDeserializer
{
public:
    virtual ~IDeserializer() = default;

    virtual std::optional<internal_types::DecodedType> decode(const internal_types::Buffer&) const = 0;
};
} // namespace internal_types
