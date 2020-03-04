#pragma once

#include <X11/Xlib.h>
#include "IKeyboardReceiver.hpp"

namespace event_consumer
{
class KeyboardReceiver : public IKeyboardReceiver
{
public:
    KeyboardReceiver(Display*);
    ~KeyboardReceiver() override;

    void onEvent(const internal_types::KeyEvent& keyEvent) const override;

private:
    Display* display;
};
} // namespace event_consumer
