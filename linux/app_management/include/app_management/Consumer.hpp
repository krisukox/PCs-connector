#pragma once

#include <functional>
#include <memory>
#include "internal_types/CommonTypes.hpp"
#include "internal_types/Point.hpp"
#include "internal_types/ScreenResolution.hpp"

namespace connection
{
class Socket;
}

namespace event_consumer
{
class IKeyboardReceiver;
class IMouseReceiver;
} // namespace event_consumer

namespace app_management
{
class Consumer : public std::enable_shared_from_this<Consumer>
{
public:
    Consumer(
        std::unique_ptr<event_consumer::IKeyboardReceiver>,
        std::unique_ptr<event_consumer::IMouseReceiver>,
        std::unique_ptr<connection::Socket>,
        std::function<void(internal_types::ScreenResolution)>);
    void start(const internal_types::ScreenResolution&);
    void setContactPoints(
        const std::pair<internal_types::Point, internal_types::Point>&,
        const internal_types::Point&,
        const internal_types::Point&);

private:
    void startReceiving();
    void handleReceivedEvent(const internal_types::Event&);

    std::unique_ptr<event_consumer::IKeyboardReceiver> keyReceiver;
    std::unique_ptr<event_consumer::IMouseReceiver> mouseReceiver;
    std::unique_ptr<connection::Socket> socket;
    std::function<void(internal_types::ScreenResolution)> setScreenResolution;
};
} // namespace app_management
