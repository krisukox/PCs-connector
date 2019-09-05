#pragma once
#include "internal_types/KeyEvent.hpp"

namespace key_management
{
class IKey
{
public:
    virtual ~IKey() = default;

    virtual void onEvent(const internal_types::KeyEvent& keyEvent) const = 0;
};
} // namespace key_management
