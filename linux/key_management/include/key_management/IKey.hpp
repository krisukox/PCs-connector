#pragma once
#include <cstddef>
#include "internal_types/KeyEvent.hpp"

namespace key_management
{
class IKey
{
public:
    virtual ~IKey() = default;

    virtual void onEvent(internal_types::KeyEvent keyEvent) const = 0;
};
} // namespace key_management
