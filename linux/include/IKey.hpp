#pragma once
#include <cstdint>

class IKey
{
public:
    virtual ~IKey() = default;
    virtual void handleEvent(std::uint16_t) = 0;
};
