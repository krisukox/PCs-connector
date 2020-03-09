#pragma once

#include <QGraphicsScene>
#include <functional>
#include <vector>
#include "GraphicsRectItem.h"

class GraphicsScene : public QGraphicsScene
{
    using QGraphicsScene::QGraphicsScene;

public:
    void addItem(QGraphicsItem* item);

private:
    void mouseReleaseEvent(QGraphicsSceneMouseEvent*) override;
    std::vector<GraphicsRectItem*> rectList;
};