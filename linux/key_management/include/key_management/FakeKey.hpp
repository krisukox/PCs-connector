#pragma once

#include <X11/Xlib.h>
#include <cstdint>
#include "internal_types/Deserializer.hpp"
#include "key_management/IKey.hpp"

namespace key_management
{
class FakeKey : public IKey
{
public:
    FakeKey(Display*);
    ~FakeKey() override;

    void onEvent(internal_types::KeyEvent keyEvent) const override;

    Display* display;

private:
    KeyCode decode(std::uint8_t);

    internal_types::Deserializer deserializer;
};
} // namespace key_management
