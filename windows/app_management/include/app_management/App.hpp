#pragma once

#include <memory>
#include "app_management/Console.hpp"
#include "commons/IApp.hpp"

namespace connection
{
} // namespace connection

namespace app_management
{
class Vendor;

class App : public commons::IApp
{
public:
    App();
    ~App() override;

    void connect(
        const boost::asio::ip::address&,
        const internal_types::ScreenResolution&,
        internal_types::SetScreenResolution&&) override;

    void setTransformationPoints(const internal_types::TransformationPoints&) override;

private:
    std::unique_ptr<Vendor> vendor;
    app_management::Console _;
};
} // namespace app_management
