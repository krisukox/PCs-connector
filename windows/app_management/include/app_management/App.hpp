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
    App(std::shared_ptr<commons::CursorGuard>&&, SetScreenResolution&&);
    ~App() override;

    void connect(const boost::asio::ip::address&, const internal_types::ScreenResolution&) override;

private:
    void initializeVendor(const internal_types::ScreenResolution&);
    void exchangeScreenResolution(
        std::shared_ptr<connection::Receiver>,
        std::shared_ptr<connection::Sender>,
        const internal_types::ScreenResolution&);

    std::unique_ptr<connection::Socket> socket;
    std::shared_ptr<Vendor> vendor;

    app_management::Console _;
};
} // namespace app_management
