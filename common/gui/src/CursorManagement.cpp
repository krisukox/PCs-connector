#include "gui/CursorManagement.h"
#include <QApplication>
#include <QCursor>
#include <QLineF>
#include <QScreen>

namespace
{
std::list<QRect> getRectSetOfScreens()
{
    std::list<QRect> rects;
    for (auto screen : qApp->screens())
    {
        rects.push_back(screen->geometry());
    }
    return rects;
}

auto screens = getRectSetOfScreens();
} // namespace

namespace CursorManagement
{
void initialize()
{
    screens = getRectSetOfScreens();
}

internal_types::Point getPosition()
{
    QPoint point = QCursor::pos();
    return internal_types::Point{static_cast<short>(point.x()), static_cast<short>(point.y())};
}

void setPosition(const internal_types::Point& point)
{
    QCursor::setPos(QPoint(point.x, point.y));
}

bool intersects(
    const internal_types::Point& a_,
    const internal_types::Point& b_,
    const internal_types::Point& c_,
    const internal_types::Point& d_)
{
    QLineF lineA(a_.x, a_.y, b_.x, b_.y);
    QLineF lineB(c_.x, c_.y, d_.x, d_.y);

    return lineA.intersects(lineB, nullptr) == QLineF::IntersectionType::BoundedIntersection;
}

bool isCursorInsideScreens(const internal_types::Point& cursor)
{
    for (auto screen : screens)
    {
        if (screen.contains(QPoint(cursor.x, cursor.y)))
        {
            return true;
        }
    }
    return false;
}
} // namespace CursorManagement
