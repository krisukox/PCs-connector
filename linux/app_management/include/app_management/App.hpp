#pragma once

#include <X11/Xlib.h>
#include <memory>
#include "Consumer.hpp"
#include "commons/IApp.hpp"
#include "connection/Socket.hpp"
#include "internal_types/Point.hpp"

namespace event_consumer
{
class IKeyboardReceiver;
} // namespace event_consumer

namespace app_management
{
// class Consumer;

class App : public commons::IApp
{
public:
    App();
    ~App() override;
    void listen(int, char* [], const internal_types::ScreenResolution&, internal_types::SetScreenResolution&&) override;
    void test();
    void setTransformationPoints(const internal_types::TransformationPoints&) override;

private:
    std::unique_ptr<event_consumer::IKeyboardReceiver> selectKeyboardReceiver(int, char* []);

    Display* display;
    std::unique_ptr<Consumer<connection::Socket>> consumer;
};
} // namespace app_management
