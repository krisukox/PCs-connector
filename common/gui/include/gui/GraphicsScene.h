#pragma once

#include <QGraphicsScene>
#include <functional>
#include <vector>
#include "GraphicsRectItem.h"

class GraphicsScene : public QGraphicsScene
{
    using QGraphicsScene::QGraphicsScene;

public:
    GraphicsScene(
        qreal x,
        qreal y,
        qreal width,
        qreal height,
        std::function<void(std::pair<QPointF, QPointF>, QPointF)>&&);
    void addItem(QGraphicsItem* item);

    std::vector<GraphicsRectItem*> rectList;

private:
    void mouseReleaseEvent(QGraphicsSceneMouseEvent*) override;
    std::function<void(std::pair<QPointF, QPointF>, QPointF)> setContactPoints;
};
