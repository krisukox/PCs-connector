#include "app_management/Vendor.hpp"
#include "event_vendor/KeyboardSender.hpp"
#include "event_vendor/MouseSender.hpp"

namespace app_management
{
Vendor::Vendor(
    std::unique_ptr<event_vendor::KeyboardSender>&& keyboardSender,
    std::unique_ptr<event_vendor::MouseSender>&& mouseSender)
    : keyboard{std::move(keyboardSender)}, mouse{std::move(mouseSender)}
{
}

void Vendor::start(std::function<void()>&& stopAppCallback)
{
    auto stopApp = [stopAppCallback = std::move(stopAppCallback)] {
        stopAppCallback();
        PostMessage(nullptr, WM_QUIT, 0, 0);
    };

    keyboard->start(std::move(stopApp));
    mouse->start([this]() { keyboard->changeState(); });
    startCatchingEvents();
}

void Vendor::startCatchingEvents()
{
    MSG msg;
    BOOL retVal;
    while ((retVal = GetMessage(&msg, nullptr, 0, 0)) != 0)
    {
        if (retVal == -1)
        {
            // handle the error and possibly exit
        }
    }
}
} // namespace app_management
