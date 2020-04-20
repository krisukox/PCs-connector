#pragma once

#include <functional>
#include "internal_types/MouseEvent.hpp"
#include "internal_types/Point.hpp"
#include "internal_types/TransformationPoints.hpp"

namespace event_consumer
{
class IMouseReceiver
{
public:
    using ForwardEvent = std::function<void(const internal_types::MouseChangePositionEvent&)>;

    virtual void onEvent(const internal_types::MouseEvent&) = 0;
    virtual void start(ForwardEvent) = 0;

    virtual void setTransformationPoints(const internal_types::TransformationPoints&) = 0;

    virtual ~IMouseReceiver() = default;
};
} // namespace event_consumer
