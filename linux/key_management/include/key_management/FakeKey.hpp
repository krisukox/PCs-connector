#pragma once

#include <X11/Xlib.h>
#include <cstdint>
#include "key_management/IKey.hpp"

namespace key_management
{
class FakeKey : public IKey
{
public:
    FakeKey();
    ~FakeKey() override;

    void handleEvent(std::uint16_t keyId) override;

private:
    Display* display;
};
} // namespace key_management
