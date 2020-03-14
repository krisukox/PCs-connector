#pragma once

#include <X11/Xlib.h>
#include <memory>
#include "interface/IApp.hpp"
#include "internal_types/Point.hpp"

namespace connection
{
class Socket;
}

namespace event_consumer
{
class IKeyboardReceiver;
class CursorGuard;
} // namespace event_consumer

namespace app_management
{
class Consumer;

class App : public IApp
{
public:
    App();
    ~App() override;
    void start(int, char* []) override;

    void setContactPoints(const std::pair<internal_types::Point, internal_types::Point>&, const internal_types::Point&)
        override;

private:
    std::shared_ptr<event_consumer::IKeyboardReceiver> keyboardReceiverSelector(int, char* []);

    Display* display;
    std::unique_ptr<connection::Socket> socket;
    std::shared_ptr<event_consumer::CursorGuard> cursorGuard;
};
} // namespace app_management
