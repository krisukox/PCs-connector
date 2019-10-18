#pragma once

#include <memory>
#include "app_management/Console.hpp"

namespace connection
{
class Socket;
class Sender;
} // namespace connection

namespace app_management
{
class Vendor;

class App
{
public:
    App(int, char*[]);
    ~App();

private:
    std::unique_ptr<connection::Socket> socket;
    std::shared_ptr<connection::Sender> sender;
    std::shared_ptr<Vendor> vendor;

    app_management::Console _;
};
} // namespace app_management