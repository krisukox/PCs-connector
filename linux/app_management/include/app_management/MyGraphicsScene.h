#pragma once

#include "MyGraphicsRectItem.h"
#include <QGraphicsScene>
#include <functional>
#include <vector>

class MyGraphicsScene : public QGraphicsScene {
  using QGraphicsScene::QGraphicsScene;

public:
  void addItem(QGraphicsItem *item);

private:
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *) override;
  std::vector<MyGraphicsRectItem *> rectList;
};
