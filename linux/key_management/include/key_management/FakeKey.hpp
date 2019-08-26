#pragma once

#include <X11/Xlib.h>
#include <cstdint>
#include "Deserializer.hpp"
#include "key_management/IKey.hpp"

namespace key_management
{
class FakeKey : public IKey
{
public:
    FakeKey();
    ~FakeKey() override;

    void handleEvent(std::byte, std::byte) const override;

    Display* display;

private:
    KeyCode decode(std::uint8_t);

    Deserializer deserializer;
};
} // namespace key_management
