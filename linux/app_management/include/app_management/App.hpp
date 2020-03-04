#pragma once

//#include <QtX11Extras/QX11Info>
#include <X11/Xlib.h>
//#include <X11/Xtos.h>
#include <memory>

// struct Display;

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
    App();
    ~App();
    void start(int, char* []);

private:
    std::shared_ptr<event_consumer::IKeyboardReceiver> keyboardReceiverSelector(int, char* []);

    Display* display;
    std::unique_ptr<connection::Socket> socket;
};
} // namespace app_management
