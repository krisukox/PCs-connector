#pragma once

#include <optional>
#include "internal_types/Point.hpp"
#include "internal_types/TransformationPoints.hpp"

namespace commons
{
class CursorGuard
{
public:
    CursorGuard();
    void initialize();

    std::optional<internal_types::MouseChangePositionEvent> checkIfCursorOutOfScreen(const internal_types::Point&);
    void setPosition(const std::optional<internal_types::MouseChangePositionEvent>& mouseEvent);

    void setTransformationPoints(const internal_types::TransformationPoints&);

private:
    bool isCursorOutOfContactArea(const internal_types::Point&);

    std::pair<internal_types::Point, internal_types::Point> contactPoints;
    internal_types::Point diffPointForSend;
    internal_types::Point diffPointForReceive;

    internal_types::Point lastCursorPosition;
};
} // namespace commons
