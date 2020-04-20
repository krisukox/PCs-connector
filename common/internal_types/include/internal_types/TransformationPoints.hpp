#pragma once

#include <utility>
#include "Point.hpp"

namespace internal_types
{
struct TransformationPoints
{
    const std::pair<internal_types::Point, internal_types::Point> contactPoints;
    const internal_types::Point diffPointForSend;
    const internal_types::Point diffPointForReceive;
};

inline bool operator==(const TransformationPoints& lv, const TransformationPoints& rv)
{
    return lv.contactPoints == rv.contactPoints && lv.diffPointForSend == rv.diffPointForSend &&
        lv.diffPointForReceive == rv.diffPointForReceive;
}
} // namespace internal_types
