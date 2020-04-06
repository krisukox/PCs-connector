#pragma once

#include <memory>
#include "app_management/Console.hpp"
#include "commons/IApp.hpp"
#include "connection/Receiver.hpp"

namespace commons
{
class CursorGuard;
}

namespace connection
{
class Socket;
class Sender;
} // namespace connection

namespace app_management
{
class Vendor;

class App : public commons::IApp
{
public:
    App();
    ~App() override;

    void connect(const boost::asio::ip::address&, const internal_types::ScreenResolution&, SetScreenResolution)
        override;

    void setContactPoints(
        const std::pair<internal_types::Point, internal_types::Point>&,
        const internal_types::Point&,
        const internal_types::Point&) override;

private:
    std::unique_ptr<Vendor> vendor;
    app_management::Console _;
};
} // namespace app_management
