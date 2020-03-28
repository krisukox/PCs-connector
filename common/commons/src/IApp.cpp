#include "commons/IApp.hpp"
#include <stdexcept>

namespace commons
{
IApp::IApp(
    std::shared_ptr<commons::CursorGuard>&& _cursorGuard,
    SetScreenResolution&& _setScreenResolution,
    const internal_types::ScreenResolution& _masterScreenResolution)
    : cursorGuard{std::move(_cursorGuard)}
    , setScreenResolution{_setScreenResolution}
    , masterScreenResolution{_masterScreenResolution}
{
}

void IApp::listen(int, char*[])
{
    throw std::runtime_error("IApp::listen method not implemented");
}

void IApp::connect(const boost::asio::ip::address&)
{
    throw std::runtime_error("IApp::connect method not implemented");
}

void IApp::setContactPoints(
    const std::pair<internal_types::Point, internal_types::Point>& contactPoints,
    const internal_types::Point& diffPointForSend,
    const internal_types::Point& diffPointForReceive)
{
    if (!cursorGuard)
    {
        throw std::runtime_error("IApp::setContactPoints - cursorGuard is uninitialized");
    }
    cursorGuard->setContactPoints(contactPoints, diffPointForSend, diffPointForReceive);
}
} // namespace commons
