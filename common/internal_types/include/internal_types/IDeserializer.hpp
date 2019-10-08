#pragma once

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

    virtual std::variant<KeyEvent, MouseEvent> decode(const internal_types::Buffer&) const = 0;
};
} // namespace internal_types
