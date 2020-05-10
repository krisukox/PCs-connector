#include "commons/CursorGuard.hpp"
#include "gui/CursorManagement.h"

namespace commons
{
CursorGuard::CursorGuard() {}

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

void CursorGuard::setTransformationPoints(const internal_types::TransformationPoints& transformationPoints)
{
    contactPoints = transformationPoints.contactPoints;
    diffPointForSend = transformationPoints.diffPointForSend;
    diffPointForReceive = transformationPoints.diffPointForReceive;
}

bool CursorGuard::isCursorOutOfContactArea(const internal_types::Point& newCursorPosition)
{
    return !CursorManagement::intersects(
        contactPoints.first, contactPoints.second, lastCursorPosition, newCursorPosition);
}
} // namespace commons
