#pragma once

#include <cstdint>

namespace internal_types
{
struct KeyEvent
{
    std::uint8_t keyCode;
    bool isPressed;
};

inline bool operator==(const internal_types::KeyEvent& lv, const internal_types::KeyEvent& rv)
{
    return (lv.keyCode == rv.keyCode && lv.isPressed == rv.isPressed);
}
} // namespace internal_types
