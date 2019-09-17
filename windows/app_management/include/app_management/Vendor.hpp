#pragma once

#include <functional>
#include <memory>

namespace event_vendor
{
class KeyboardSender;
class MouseSender;
} // namespace event_vendor

namespace app_management
{
class Vendor
{
public:
    Vendor(std::unique_ptr<event_vendor::KeyboardSender>&&, std::unique_ptr<event_vendor::MouseSender>&&);

    void start(std::function<void()>&&);

private:
    void startCatchingEvents();

    std::unique_ptr<event_vendor::KeyboardSender> keyboard;
    std::unique_ptr<event_vendor::MouseSender> mouse;
};
} // namespace app_management
