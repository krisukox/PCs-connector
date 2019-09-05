#pragma once

#include <variant>
#include "KeyEvent.hpp"
#include "MouseEvent.hpp"
#include "server_app/ServerAppTypes.hpp"

namespace internal_types
{
class IDeserializer
{
public:
    virtual ~IDeserializer() = default;

    virtual std::variant<KeyEvent, MouseEvent> decode(const server_app::Buffer&) const = 0;
};
} // namespace internal_types
