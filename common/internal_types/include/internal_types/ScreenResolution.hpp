#pragma once
#include <cstdint>

namespace internal_types
{
struct ScreenResolution
{
    std::uint16_t width;
    std::uint16_t height;
};

inline bool operator==(const ScreenResolution& lv, const ScreenResolution& rv)
{
    return (lv.width == rv.width && lv.height == rv.height);
}
} // namespace internal_types
