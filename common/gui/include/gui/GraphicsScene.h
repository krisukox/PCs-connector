#pragma once

#include <QGraphicsScene>
#include <functional>
#include <vector>
#include "GraphicsRectItem.h"

class GraphicsScene : public QGraphicsScene
{
    using QGraphicsScene::QGraphicsScene;

public:
    using SetContactPoints = std::function<void(std::pair<QPointF, QPointF>&, QPointF&)>;
    GraphicsScene(qreal x, qreal y, qreal width, qreal height, SetContactPoints&&);

private:
    void mouseReleaseEvent(QGraphicsSceneMouseEvent*) override;
    SetContactPoints setContactPoints;
};
