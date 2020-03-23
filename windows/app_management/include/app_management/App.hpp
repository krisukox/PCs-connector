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
    App(std::shared_ptr<commons::CursorGuard>&&, SetScreenResolution&&, const internal_types::ScreenResolution&);
    ~App() override;

    void connect(const boost::asio::ip::address&) override;

private:
    void initializeVendor();
    void exchangeScreenResolution(std::shared_ptr<connection::Receiver>, std::shared_ptr<connection::Sender>);

    std::unique_ptr<connection::Socket> socket;
    std::shared_ptr<Vendor> vendor;

    app_management::Console _;
};
} // namespace app_management
