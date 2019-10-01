#pragma once

#include <X11/Xlib.h>
#include "IKeyboardReceiver.hpp"

namespace event_consumer
{
class TestKeyboardReceiver : public IKeyboardReceiver
{
public:
    TestKeyboardReceiver();
    ~TestKeyboardReceiver() override;

    void onEvent(const internal_types::KeyEvent& keyEvent) const override;
};
} // namespace event_consumer
