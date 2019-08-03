#pragma once

#include <X11/Xlib.h>
#include <cstdint>
#include "../include/IKey.hpp"

class FakeKey : public IKey
{
public:
    FakeKey();
    ~FakeKey() override;
    void handleEvent(std::uint16_t keyId) override;

private:
    Display* display;
};
