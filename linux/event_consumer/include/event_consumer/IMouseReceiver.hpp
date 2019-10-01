#pragma once

#include "internal_types/MouseEvent.hpp"

namespace event_consumer
{
class IMouseReceiver
{
public:
    virtual void onEvent(const internal_types::MouseEvent&) = 0;

    virtual ~IMouseReceiver() = default;
};
} // namespace event_consumer
