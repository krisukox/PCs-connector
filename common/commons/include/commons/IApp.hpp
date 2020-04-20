#pragma once

#include <boost/asio.hpp>
#include <memory>
#include <utility>
#include "CursorGuard.hpp"
#include "internal_types/CommonTypes.hpp"
#include "internal_types/Point.hpp"
#include "internal_types/ScreenResolution.hpp"
#include "internal_types/TransformationPoints.hpp"

namespace commons
{
class IApp
{
public:
    virtual ~IApp() = default;

    virtual void listen(int, char* [], const internal_types::ScreenResolution&, internal_types::SetScreenResolution&&);
    virtual void connect(
        const boost::asio::ip::address&,
        const internal_types::ScreenResolution&,
        internal_types::SetScreenResolution&&);
    virtual void setTransformationPoints(const internal_types::TransformationPoints&) = 0;
};
} // namespace commons
