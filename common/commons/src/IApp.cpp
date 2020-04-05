#include "commons/IApp.hpp"
#include <stdexcept>

namespace commons
{
void IApp::listen(int, char*[], const internal_types::ScreenResolution&)
{
    throw std::runtime_error("IApp::listen method not implemented");
}

void IApp::connect(const boost::asio::ip::address&, const internal_types::ScreenResolution&, SetScreenResolution)
{
    throw std::runtime_error("IApp::connect method not implemented");
}

// void IApp::setContactPoints(
//    const std::pair<internal_types::Point, internal_types::Point>& contactPoints,
//    const internal_types::Point& diffPointForSend,
//    const internal_types::Point& diffPointForReceive)
//{
//    if (!cursorGuard)
//    {
//        throw std::runtime_error("IApp::setContactPoints - cursorGuard is uninitialized");
//    }
//    cursorGuard->setContactPoints(contactPoints, diffPointForSend, diffPointForReceive);
//}
} // namespace commons
