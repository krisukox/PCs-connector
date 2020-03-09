#include <QDebug>
#include <QGraphicsEllipseItem>
#include <QGraphicsItem>
#include <QGraphicsLineItem>
#include <QLineF>
#include <QMouseEvent>
#include <cmath>
#include <iostream>
#include <optional>
#include "app_management/GraphicsRectItem.h"
#include "app_management/GraphicsScene.h"

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

std::optional<QPointF> intersectPoint(const QRectF& rect, const QLineF& line)
{
    return intersect(
        {QLineF(rect.topLeft(), rect.topRight()),
         QLineF(rect.topRight(), rect.bottomRight()),
         QLineF(rect.bottomRight(), rect.bottomLeft()),
         QLineF(rect.bottomLeft(), rect.topLeft())},
        line);
}
} // namespace

void GraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    if (rectList.size() != 2) return;

    auto subRect1 = rectList.at(0)->rectPlaced();
    auto subRect2 = rectList.at(1)->rectPlaced();

    auto lineBetweenCenters = QLineF(subRect1.center(), subRect2.center());

    if (subRect1.intersects(subRect2))
    {
        auto intersectedRect = subRect1.intersected(subRect2);
        auto normalLine = normalizeLine(lineBetweenCenters);

        auto diffLine = intersectLine(intersectedRect, normalLine);
        if (diffLine)
        {
            if (QLineF(subRect1.center() + QPointF(diffLine->dx(), diffLine->dy()), subRect2.center()).length() <
                QLineF(subRect1.center() - QPointF(diffLine->dx(), diffLine->dy()), subRect2.center()).length())
            {
                rectList.at(0)->moveBy(-diffLine->dx(), -diffLine->dy());
            }
            else
            {
                rectList.at(0)->moveBy(diffLine->dx(), diffLine->dy());
            }
        }
    }
    else
    {
        auto point1 = intersectPoint(subRect1, lineBetweenCenters);
        auto point2 = intersectPoint(subRect2, lineBetweenCenters);
        if (point1 && point2)
        {
            QLineF diffLine(point1.value(), point2.value());
            rectList.at(0)->moveBy(diffLine.dx(), diffLine.dy());
        }
    }
    if (rectList.at(0)->rectPlaced().intersects(rectList.at(1)->rectPlaced()))
    {
        mouseReleaseEvent(event);
        return;
    }
    auto rect1 = rectList.at(0)->rectPlaced();
    auto rect2 = rectList.at(1)->rectPlaced();
    qDebug() << "rect1: " << rect1.topLeft() << " " << rect1.topRight() << " " << rect1.bottomRight() << " "
             << rect1.bottomLeft();
    qDebug() << "rect2: " << rect2.topLeft() << " " << rect2.topRight() << " " << rect2.bottomRight() << " "
             << rect2.bottomLeft();
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
