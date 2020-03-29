#pragma once

#include <optional>
#include "internal_types/Point.hpp"

namespace commons
{
class CursorGuard
{
public:
    CursorGuard();
    void initialize();

    std::optional<internal_types::MouseChangePositionEvent> checkIfCursorOutOfScreen(const internal_types::Point&);
    void setPosition(const std::optional<internal_types::MouseChangePositionEvent>& mouseEvent);

    void setContactPoints(
        const std::pair<internal_types::Point, internal_types::Point>& contactPoints,
        const internal_types::Point& diffPointForSend,
        const internal_types::Point& diffPointForReceive);

private:
    bool isCursorInsideScreen(const internal_types::Point&);
    bool isCursorOutOfContactArea(const internal_types::Point&);

    std::pair<internal_types::Point, internal_types::Point> contactPoints;
    internal_types::Point diffPointForSend;
    internal_types::Point diffPointForReceive;

    internal_types::Point lastCursorPosition;
};
} // namespace commons
