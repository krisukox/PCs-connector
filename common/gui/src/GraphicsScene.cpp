#include "gui/GraphicsScene.h"
#include <QLineF>
#include <QtDebug>
#include <optional>
#include <set>
#include "gui/GraphicsRectItem.h"

namespace
{
QLineF normalizeLine(const QLineF& line)
{
    auto ret_line = line;
    ret_line.setLength(150);
    ret_line.setPoints(ret_line.p2(), ret_line.p1());
    ret_line.setLength(300);
    return ret_line;
}

std::optional<QPointF> intersect(std::vector<QLineF> lines, const QLineF& intersectLine)
{
    QPointF sub_point;
    for (auto& line : lines)
    {
        if (line.intersects(intersectLine, &sub_point) == QLineF::IntersectType::BoundedIntersection)
        {
            return sub_point;
        }
    }
    return std::nullopt;
}

std::optional<QPointF> intersectPoint(const QRectF& rect, const QLineF& line)
{
    return intersect(
        {QLineF(rect.topLeft(), rect.topRight()),
         QLineF(rect.topRight(), rect.bottomRight()),
         QLineF(rect.bottomRight(), rect.bottomLeft()),
         QLineF(rect.bottomLeft(), rect.topLeft())},
        line);
}

struct QPointCmp
{
    bool operator()(const QPointF& point1, const QPointF& point2) const
    {
        if (point1.x() == point2.x())
        {
            return point1.y() < point2.y();
        }
        else
        {
            return point1.x() < point2.x();
        }
    }
};

std::set<QPointF, QPointCmp> getTwoLinearPoints(std::vector<QPointF>& rect1Points, std::vector<QPointF>& rect2Points)
{
    for (auto point1 = rect1Points.begin(); point1 != rect1Points.end(); point1++)
    {
        for (auto point2 = rect2Points.begin(); point2 != rect2Points.end(); point2++)
        {
            if (point1->x() == point2->x() || point1->y() == point2->y())
            {
                std::set<QPointF, QPointCmp> s{*point1, *point2};
                rect1Points.erase(point1);
                rect2Points.erase(point2);

                return s;
            }
        }
    }
    return {};
}

void addLinearPoint(const std::vector<QPointF>& rectPoints, std::set<QPointF, QPointCmp>& linearPoints)
{
    auto linearPoint = linearPoints.begin();
    auto linearPoint2 = linearPoint++;

    for (const auto& point : rectPoints)
    {
        if (linearPoint->x() == linearPoint2->x())
        {
            if (point.x() == linearPoint->x())
            {
                linearPoints.insert(point);
                return;
            }
        }
        else
        {
            if (point.y() == linearPoint->y())
            {
                linearPoints.insert(point);
                return;
            }
        }
    }
}

std::vector<QPointF> getSamePoints(const std::vector<QPointF>& rect1Points, const std::vector<QPointF>& rect2Points)
{
    std::vector<QPointF> retVal;
    for (auto point1 = rect1Points.begin(); point1 != rect1Points.end(); point1++)
    {
        for (auto point2 = rect2Points.begin(); point2 != rect2Points.end(); point2++)
        {
            if (*point1 == *point2)
            {
                retVal.push_back(*point1);
            }
        }
    }
    return retVal;
}

std::pair<QPointF, QPointF> getContactPoints(const QRectF& rect1, const QRectF& rect2)
{
    std::vector<QPointF> rect1Points{rect1.topLeft(), rect1.topRight(), rect1.bottomLeft(), rect1.bottomRight()};
    std::vector<QPointF> rect2Points{rect2.topLeft(), rect2.topRight(), rect2.bottomLeft(), rect2.bottomRight()};

    auto samePoints = getSamePoints(rect1Points, rect2Points);
    if (samePoints.size() == 2)
    {
        return {samePoints.at(0), samePoints.at(1)};
    }

    auto linearPoints = getTwoLinearPoints(rect1Points, rect2Points);

    addLinearPoint(rect1Points, linearPoints);
    addLinearPoint(rect2Points, linearPoints);

    auto pointIt1 = ++linearPoints.begin();
    auto pointIt2 = pointIt1;
    pointIt2++;

    return {*pointIt1, *pointIt2};
}

void alignPointsToScreen(std::pair<QPointF, QPointF>& contactPoints, const GraphicsRectItem* masterRect)
{
    contactPoints.first -= masterRect->rectPlaced().topLeft();
    contactPoints.second -= masterRect->rectPlaced().topLeft();
}

QPointF diffPointAlignedToScreen(const GraphicsRectItem* masterScreen, const GraphicsRectItem* slaveScreen)
{
    return masterScreen->pos() - slaveScreen->pos();
}

std::optional<QLineF> intersectLine(const QRectF& rect, const QLineF& line)
{
    auto point1 =
        intersect({QLineF(rect.topLeft(), rect.topRight()), QLineF(rect.topRight(), rect.bottomRight())}, line);
    auto point2 =
        intersect({QLineF(rect.bottomRight(), rect.bottomLeft()), QLineF(rect.bottomLeft(), rect.topLeft())}, line);
    if (point1 && point2)
    {
        return QLineF(point1.value(), point2.value());
    }
    return std::nullopt;
}
} // namespace

GraphicsScene::GraphicsScene(
    qreal x,
    qreal y,
    qreal width,
    qreal height,
    std::function<void(std::pair<QPointF, QPointF>&, QPointF&)>&& setContactPoints_)
    : QGraphicsScene{x, y, width, height}, setContactPoints{setContactPoints_}
{
}

void GraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    if (items().size() != 2) return;

    auto rect1_ = dynamic_cast<GraphicsRectItem*>(items().at(0));
    auto rect2_ = dynamic_cast<GraphicsRectItem*>(items().at(1));

    if (!rect1_ || !rect2_) return;

    auto rect1 = rect1_->rectPlaced();
    auto rect2 = rect2_->rectPlaced();

    auto lineBetweenCenters = QLineF(rect1.center(), rect2.center());

    if (rect1.intersects(rect2))
    {
        auto intersectedRect = rect1.intersected(rect2);
        auto normalLine = normalizeLine(lineBetweenCenters);

        auto diffLine = intersectLine(intersectedRect, normalLine);

        if (diffLine)
        {
            if (QLineF(rect1.center() + QPointF(diffLine->dx(), diffLine->dy()), rect2.center()).length() <
                QLineF(rect1.center() - QPointF(diffLine->dx(), diffLine->dy()), rect2.center()).length())
            {
                items().at(0)->moveBy(-diffLine->dx(), -diffLine->dy());
            }
            else
            {
                items().at(0)->moveBy(diffLine->dx(), diffLine->dy());
            }
        }
    }
    else
    {
        auto point1 = intersectPoint(rect1, lineBetweenCenters);
        auto point2 = intersectPoint(rect2, lineBetweenCenters);
        if (point1 && point2)
        {
            QLineF diffLine(point1.value(), point2.value());
            items().at(0)->moveBy(diffLine.dx(), diffLine.dy());
        }
    }
    rect1 = rect1_->rectPlaced();
    rect2 = rect2_->rectPlaced();

    if (rect1.intersects(rect2))
    {
        mouseReleaseEvent(event);
        return;
    }

    auto contactPoints = getContactPoints(rect1, rect2);
    QPointF diffPoint;

    if (rect1_->type() == GraphicsRectItem::ScreenType::master)
    {
        alignPointsToScreen(contactPoints, rect1_);
        diffPoint = diffPointAlignedToScreen(rect1_, rect2_);
    }
    else
    {
        alignPointsToScreen(contactPoints, rect2_);
        diffPoint = diffPointAlignedToScreen(rect2_, rect1_);
    }

    setContactPoints(contactPoints, diffPoint);
    QGraphicsScene::mouseReleaseEvent(event);
}

void GraphicsScene::addItem(QGraphicsItem* item)
{
    auto rectItem = dynamic_cast<GraphicsRectItem*>(item);
    if (rectItem)
    {
        rectList.push_back(rectItem);
    }
    QGraphicsScene::addItem(item);
}
