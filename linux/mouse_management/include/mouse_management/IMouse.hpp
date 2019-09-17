#pragma once

#include "internal_types/MouseEvent.hpp"

namespace mouse_management
{
class IMouse
{
public:
    virtual void onEvent(const internal_types::MouseEvent&) = 0;

    virtual ~IMouse() = default;
};
} // namespace mouse_management
