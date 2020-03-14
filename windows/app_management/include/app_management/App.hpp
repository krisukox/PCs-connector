#pragma once

#include <memory>
#include "app_management/Console.hpp"
#include "interface/IApp.hpp"

namespace connection
{
class Socket;
class Sender;
} // namespace connection

namespace app_management
{
class Vendor;

class App : public IApp
{
public:
    App();
    ~App() override;

    void start(int, char*[]) override;
    void setContactPoints(const std::pair<internal_types::Point, internal_types::Point>&, const internal_types::Point&)
        override;

private:
    void initializeVendor();

    std::unique_ptr<connection::Socket> socket;
    std::shared_ptr<Vendor> vendor;

    app_management::Console _;
};
} // namespace app_management
