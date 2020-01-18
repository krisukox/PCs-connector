#pragma once

#include <X11/Xlib.h>
#include <memory>

namespace connection
{
class Socket;
}

namespace event_consumer
{
class IKeyboardReceiver;
}

namespace app_management
{
class Consumer;

class App
{
public:
    App(int, char* []);
    ~App();

private:
    std::shared_ptr<event_consumer::IKeyboardReceiver> keyboardReceiverSelector(int, char* []);

    Display* display;
    std::unique_ptr<connection::Socket> socket;
};
} // namespace app_management
