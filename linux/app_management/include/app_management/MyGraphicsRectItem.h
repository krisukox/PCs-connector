#pragma once

#include <QGraphicsRectItem>
#include <functional>

class MyGraphicsRectItem : public QGraphicsRectItem
{
    using QGraphicsRectItem::QGraphicsRectItem;

public:
    void setCallback(std::function<void()>&&);
    QRectF rectPlaced();

private:
    void mousePressEvent(QGraphicsSceneMouseEvent*) override;

    std::function<void()> callbackSetSecondItemDown;
};
