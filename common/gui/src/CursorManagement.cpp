#include "gui/CursorManagement.h"
#include <QCursor>

namespace CursorManagement
{
internal_types::Point getPosition()
{
    QPoint point = QCursor::pos();
    return internal_types::Point{static_cast<short>(point.x()), static_cast<short>(point.y())};
}

void setPosition(const internal_types::Point& point)
{
    QCursor::setPos(QPoint(point.x, point.y));
}
} // namespace CursorManagement
