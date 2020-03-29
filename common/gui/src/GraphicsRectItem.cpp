#include "gui/GraphicsRectItem.h"
#include <QGraphicsSceneMouseEvent>
#include <iostream>

GraphicsRectItem::GraphicsRectItem(const QRectF& rect, ScreenType _screenType)
    : QGraphicsRectItem{rect}, screenType{_screenType}
{
}

void GraphicsRectItem::setCallback(std::function<void()>&& callback)
{
    callbackSetSecondItemDown = std::move(callback);
}

QRectF GraphicsRectItem::rectPlaced() const
{
    auto ownRect = rect();
    ownRect.moveTo(pos());
    return ownRect;
}

GraphicsRectItem::ScreenType GraphicsRectItem::type()
{
    return screenType;
}

void GraphicsRectItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    setZValue(1);
    callbackSetSecondItemDown();
    QGraphicsItem::mousePressEvent(event);
}
