#pragma once

#include <X11/Xlib.h>
#include <cstdint>
#include "key_management/IKey.hpp"

namespace key_management
{
class FakeKey : public IKey
{
public:
    FakeKey(Display*);
    ~FakeKey() override;

    void onEvent(internal_types::KeyEvent keyEvent) const override;

private:
    KeyCode decode(std::uint8_t);

    Display* display;
};
} // namespace key_management
