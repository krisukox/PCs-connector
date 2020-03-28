#pragma once

#include "internal_types/Point.hpp"

namespace CursorManagement
{
void initialize();
internal_types::Point getPosition();
void setPosition(const internal_types::Point&);
bool intersects(
    const internal_types::Point&,
    const internal_types::Point&,
    const internal_types::Point&,
    const internal_types::Point&);
bool isCursorInsideScreens(const internal_types::Point&);
} // namespace CursorManagement
