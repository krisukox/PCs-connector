#pragma once

#include <X11/Xlib.h>
#include <memory>
#include "commons/IApp.hpp"
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

class App : public commons::IApp
{
public:
    App();
    ~App() override;
    void start(int, char* []) override;

private:
    std::shared_ptr<event_consumer::IKeyboardReceiver> keyboardReceiverSelector(int, char* []);

    Display* display;
    std::unique_ptr<connection::Socket> socket;
};
} // namespace app_management
