#pragma once
#include <cstddef>

namespace key_management
{
class IKey
{
public:
    virtual ~IKey() = default;

    virtual void handleEvent(std::byte, std::byte) const = 0;
};
} // namespace key_management
