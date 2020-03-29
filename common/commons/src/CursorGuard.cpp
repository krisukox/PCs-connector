#include "commons/CursorGuard.hpp"
#include <iostream>
#include "gui/CursorManagement.h"

namespace commons
{
CursorGuard::CursorGuard(const int _widthOfScreen, const int _heightOfScreen)
    : widthOfScreen{_widthOfScreen}, heightOfScreen{_heightOfScreen}
{
}

void CursorGuard::initialize()
{
    lastCursorPosition = CursorManagement::getPosition();
}

std::optional<internal_types::MouseChangePositionEvent> CursorGuard::checkIfCursorOutOfScreen(
    const internal_types::Point& cursor)
{
    if (CursorManagement::isCursorInsideScreens(cursor))
    {
        lastCursorPosition = cursor;
        return std::nullopt;
    }
    if (isCursorOutOfContactArea(cursor))
    {
        return std::nullopt;
    }
    return internal_types::MouseChangePositionEvent{static_cast<short>(cursor.x + diffPointForSend.x),
                                                    static_cast<short>(cursor.y + diffPointForSend.y)};
}

void CursorGuard::setPosition(const std::optional<internal_types::MouseChangePositionEvent>& mouseEvent)
{
    if (mouseEvent)
    {
        lastCursorPosition = internal_types::Point{mouseEvent->x, mouseEvent->y} + diffPointForReceive;
        CursorManagement::setPosition(internal_types::Point{mouseEvent->x, mouseEvent->y} + diffPointForReceive);
    }
    else
    {
        CursorManagement::setPosition({0, 0});
    }
}

void CursorGuard::setContactPoints(
    const std::pair<internal_types::Point, internal_types::Point>& contactPoints_,
    const internal_types::Point& diffPointForSend_,
    const internal_types::Point& diffPointForReceive_)
{
    contactPoints = contactPoints_;
    diffPointForSend = diffPointForSend_;
    diffPointForReceive = diffPointForReceive_;
}

bool CursorGuard::isCursorInsideScreen(const internal_types::Point& cursor)
{
    return cursor.x < widthOfScreen && cursor.x >= 0 && cursor.y < heightOfScreen && cursor.y >= 0;
}

bool CursorGuard::isCursorOutOfContactArea(const internal_types::Point& newCursorPosition)
{
    return !CursorManagement::intersects(
        contactPoints.first, contactPoints.second, lastCursorPosition, newCursorPosition);
}
} // namespace commons
