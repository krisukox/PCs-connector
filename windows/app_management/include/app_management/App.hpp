#pragma once

#include <memory>
#include "app_management/Console.hpp"
#include "commons/IApp.hpp"

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
    App(std::shared_ptr<commons::CursorGuard>&&);
    ~App() override;

    void start(int, char*[]) override;

private:
    void initializeVendor();

    std::unique_ptr<connection::Socket> socket;
    std::shared_ptr<Vendor> vendor;

    app_management::Console _;
};
} // namespace app_management
