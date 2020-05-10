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
class App : public commons::IApp
{
public:
    App();
    ~App() override;
    void listen(const internal_types::ScreenResolution&, internal_types::SetScreenResolution&&) override;
    void test(const internal_types::ScreenResolution&);
    void setTransformationPoints(const internal_types::TransformationPoints&) override;

private:
    std::unique_ptr<event_consumer::IKeyboardReceiver> selectKeyboardReceiver(int, char* []);
    std::unique_ptr<connection::Socket> createSocket();

    Display* display;
    std::unique_ptr<Consumer<connection::Socket>> consumer;
};
} // namespace app_management
