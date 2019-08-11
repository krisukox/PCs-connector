#pragma once
#include <cstdint>

namespace key_management
{
class IKey
{
public:
    virtual ~IKey() = default;
    virtual void handleEvent(std::uint16_t) const = 0;
};
} // namespace key_management
