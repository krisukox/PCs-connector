#pragma once

#include <X11/Xlib.h>

namespace internal_types
{
struct KeyEvent
{
    KeyCode keyCode;
    bool isPressed;
};

inline bool operator==(const internal_types::KeyEvent& lv, const internal_types::KeyEvent& rv)
{
    return lv.keyCode == rv.keyCode && lv.isPressed == rv.isPressed;
}
} // namespace internal_types
