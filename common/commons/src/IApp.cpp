#include "commons/IApp.hpp"
#include <stdexcept>

namespace commons
{
IApp::IApp(std::shared_ptr<commons::CursorGuard>&& _cursorGuard) : cursorGuard{std::move(_cursorGuard)} {}

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
