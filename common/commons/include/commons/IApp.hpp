#pragma once

#include <boost/asio.hpp>
#include <memory>
#include <utility>
#include "CursorGuard.hpp"
#include "internal_types/Point.hpp"
#include "internal_types/ScreenResolution.hpp"

namespace commons
{
class IApp
{
public:
    using SetScreenResolution = std::function<void(internal_types::ScreenResolution)>;

    virtual ~IApp() = default;
    IApp(std::shared_ptr<commons::CursorGuard>&&, SetScreenResolution&&);

    virtual void connect(const boost::asio::ip::address&);
    virtual void listen(int, char* [], const internal_types::ScreenResolution&);
    void setContactPoints(const std::pair<internal_types::Point, internal_types::Point>&, const internal_types::Point&);

protected:
    std::shared_ptr<CursorGuard> cursorGuard;
    SetScreenResolution setScreenResolution;
};
} // namespace commons
