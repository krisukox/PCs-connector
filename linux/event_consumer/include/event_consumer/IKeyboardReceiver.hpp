#pragma once
#include "internal_types/KeyEvent.hpp"

namespace event_consumer
{
class IKeyboardReceiver
{
public:
    virtual ~IKeyboardReceiver() = default;

    virtual void onEvent(const internal_types::KeyEvent& keyEvent) const = 0;
};
} // namespace event_consumer
