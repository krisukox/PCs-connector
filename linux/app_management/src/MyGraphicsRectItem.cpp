#include "app_management/MyGraphicsRectItem.h"
#include <QGraphicsSceneMouseEvent>
#include <iostream>

void MyGraphicsRectItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    setZValue(1);
    callbackSetSecondItemDown();
    QGraphicsItem::mousePressEvent(event);
}

void MyGraphicsRectItem::setCallback(std::function<void()>&& callback)
{
    callbackSetSecondItemDown = std::move(callback);
}

QRectF MyGraphicsRectItem::rectPlaced()
{
    auto ownRect = rect();
    ownRect.moveTo(pos());
    return ownRect;
}
