#pragma once

#include <X11/Xlib.h>
#include <memory>
#include "commons/IApp.hpp"
#include "internal_types/Point.hpp"

namespace event_consumer
{
class IKeyboardReceiver;
} // namespace event_consumer

namespace app_management
{
class Consumer;

class App : public commons::IApp
{
public:
    App();
    ~App() override;
    void listen(int, char* [], const internal_types::ScreenResolution&, internal_types::SetScreenResolution&&) override;
    void setContactPoints(
        const std::pair<internal_types::Point, internal_types::Point>& contactPoints,
        const internal_types::Point& diffPointForSend,
        const internal_types::Point& diffPointForReceive) override;

private:
    std::unique_ptr<event_consumer::IKeyboardReceiver> selectKeyboardReceiver(int, char* []);

    Display* display;
    std::unique_ptr<Consumer> consumer;
};
} // namespace app_management
