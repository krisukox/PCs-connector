#pragma once

#include <X11/Xlib.h>
#include <unordered_map>

namespace key_management
{
class Deserializer
{
public:
    Deserializer(Display*);

    KeyCode decode(const std::byte keyId) const;

private:
    Display* display;
    const std::unordered_map<std::byte, KeyCode> translationTabel;
};
} // namespace key_management
