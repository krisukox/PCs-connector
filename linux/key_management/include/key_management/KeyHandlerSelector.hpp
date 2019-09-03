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

    std::shared_ptr<IKey> select(int, char* []);

private:
    Display* display;
};
} // namespace key_management
