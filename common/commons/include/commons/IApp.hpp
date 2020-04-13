#pragma once

#include <boost/asio.hpp>
#include <memory>
#include <utility>
#include "CursorGuard.hpp"
#include "internal_types/CommonTypes.hpp"
#include "internal_types/Point.hpp"
#include "internal_types/ScreenResolution.hpp"

namespace commons
{
class IApp
{
public:
    virtual ~IApp() = default;

    virtual void listen(int, char*[], const internal_types::ScreenResolution&, internal_types::SetScreenResolution&&);
    virtual void connect(
        const boost::asio::ip::address&,
        const internal_types::ScreenResolution&,
        internal_types::SetScreenResolution&&);
    virtual void setContactPoints(
        const std::pair<internal_types::Point, internal_types::Point>& contactPoints,
        const internal_types::Point& diffPointForSend,
        const internal_types::Point& diffPointForReceive) = 0;
};
} // namespace commons
