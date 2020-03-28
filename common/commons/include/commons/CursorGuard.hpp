#pragma once

#include <optional>
#include "internal_types/Point.hpp"

namespace commons
{
class CursorGuard
{
public:
    CursorGuard(const int widthOfScreen, const int heightOfScreen);
    std::optional<internal_types::MouseChangePositionEvent> checkIfCursorOutOfScreen();
    bool setPosition(const std::optional<internal_types::MouseChangePositionEvent>& mouseEvent);

    void setContactPoints(const std::pair<internal_types::Point, internal_types::Point>&, const internal_types::Point&);

private:
    bool isCursorInsideScreen(const internal_types::Point&);
    bool isCursorOutOfContactArea(const internal_types::Point&);

    const int widthOfScreen;
    const int heightOfScreen;

    std::pair<internal_types::Point, internal_types::Point> contactPoints;
    internal_types::Point diffPoint;
};
} // namespace commons
