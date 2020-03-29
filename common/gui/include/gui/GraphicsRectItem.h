#pragma once

#include <QGraphicsRectItem>
#include <functional>

class GraphicsRectItem : public QGraphicsRectItem
{
    using QGraphicsRectItem::QGraphicsRectItem;

public:
    enum class ScreenType
    {
        master,
        slave
    };

    GraphicsRectItem(const QRectF& rect, ScreenType screenType);

    void setCallback(std::function<void()>&&);
    QRectF rectPlaced() const;
    ScreenType type();

private:
    void mousePressEvent(QGraphicsSceneMouseEvent*) override;

    std::function<void()> callbackSetSecondItemDown;
    const ScreenType screenType;
};
