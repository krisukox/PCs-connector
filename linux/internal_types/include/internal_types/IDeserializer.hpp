#pragma once

#include "KeyEvent.hpp"
#include "server_app/ServerAppTypes.hpp"

namespace internal_types
{
class IDeserializer
{
public:
    virtual ~IDeserializer() = default;

    virtual KeyEvent decode(const server_app::Buffer&) const = 0;
};
} // namespace internal_types
