#pragma once

#include <X11/Xlib.h>
#include <memory>

namespace key_management
{
class IKey;

class KeyHandlerSelector
{
public:
    KeyHandlerSelector(Display* display);

    Display* display;
    std::shared_ptr<IKey> select(int, char* []);
};
} // namespace key_management
