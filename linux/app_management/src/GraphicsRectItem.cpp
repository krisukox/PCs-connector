#include "app_management/GraphicsRectItem.h"
#include <QGraphicsSceneMouseEvent>
#include <iostream>

void GraphicsRectItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    setZValue(1);
    callbackSetSecondItemDown();
    QGraphicsItem::mousePressEvent(event);
}

void GraphicsRectItem::setCallback(std::function<void()>&& callback)
{
    callbackSetSecondItemDown = std::move(callback);
}

QRectF GraphicsRectItem::rectPlaced()
{
    auto ownRect = rect();
    ownRect.moveTo(pos());
    return ownRect;
}
