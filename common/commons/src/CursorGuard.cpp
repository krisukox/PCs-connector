#include "commons/CursorGuard.hpp"

namespace commons
{
CursorGuard::CursorGuard(const int _widthOfScreen, const int _heightOfScreen)
    : widthOfScreen{_widthOfScreen}, heightOfScreen{_heightOfScreen}
{
}

std::optional<internal_types::MouseChangePositionEvent> CursorGuard::checkIfCursorOutOfScreen(
    const internal_types::Point& cursor)
{
    if (isCursorInsideScreen(cursor) || isCursorOutOfContactArea(cursor))
    {
        return std::nullopt;
    }
    // HARDCODE
    return internal_types::MouseChangePositionEvent{1919, static_cast<short>(cursor.y + diffPoint.y)};
    // END HARDCODE
    //    return internal_types::MouseChangePositionEvent{static_cast<short>(cursor.x + diffPoint.x),
    //                                                    static_cast<short>(cursor.y + diffPoint.y)};
}

void CursorGuard::setContactPoints(
    const std::pair<internal_types::Point, internal_types::Point>& contactPoints_,
    const internal_types::Point& diffPoint_)
{
    contactPoints = contactPoints_;
    diffPoint = diffPoint_;
}

bool CursorGuard::isCursorInsideScreen(const internal_types::Point& cursor)
{
    return cursor.x < widthOfScreen && cursor.x >= 0 && cursor.y < heightOfScreen && cursor.y >= 0;
}

bool CursorGuard::isCursorOutOfContactArea(const internal_types::Point& cursor)
{
    if (contactPoints.first.x == contactPoints.second.x)
    {
        if (contactPoints.first.x == 0)
        {
            if (cursor.x > 0)
            {
                return true;
            }
        }
        else if (cursor.x < 0)
        {
            return true;
        }
        if (contactPoints.first.y < contactPoints.second.y)
        {
            return cursor.y < contactPoints.first.y || cursor.y > contactPoints.second.y;
        }
        return cursor.y > contactPoints.first.y || cursor.y < contactPoints.second.y;
    }
    if (contactPoints.first.y == 0)
    {
        if (cursor.y > 0)
        {
            return true;
        }
    }
    else if (cursor.y < 0)
    {
        return true;
    }
    if (contactPoints.first.x < contactPoints.second.x)
    {
        return cursor.x < contactPoints.first.x || cursor.x > contactPoints.second.x;
    }
    return cursor.x > contactPoints.first.x || cursor.x < contactPoints.second.x;
}
} // namespace commons
