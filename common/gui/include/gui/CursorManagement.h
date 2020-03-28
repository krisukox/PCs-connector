#pragma once

#include "internal_types/Point.hpp"

namespace CursorManagement
{
internal_types::Point getPosition();
void setPosition(const internal_types::Point&);
} // namespace CursorManagement
