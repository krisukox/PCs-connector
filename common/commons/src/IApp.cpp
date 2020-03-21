#include "commons/IApp.hpp"
#include <stdexcept>

namespace commons
{
IApp::IApp(std::shared_ptr<commons::CursorGuard>&& _cursorGuard) : cursorGuard{std::move(_cursorGuard)} {}

void IApp::listen(int, char* [], SetScreenResolution, const internal_types::ScreenResolution&)
{
    throw std::runtime_error("IApp::listen method not implemented");
}

void IApp::connect(const boost::asio::ip::address&)
{
    throw std::runtime_error("IApp::connect method not implemented");
}

void IApp::setContactPoints(
    const std::pair<internal_types::Point, internal_types::Point>& contactPoints,
    const internal_types::Point& diffPoint)
{
    if (!cursorGuard)
    {
        throw std::runtime_error("IApp::setContactPoints - cursorGuard is uninitialized");
    }
    cursorGuard->setContactPoints(contactPoints, diffPoint);
}
} // namespace commons
