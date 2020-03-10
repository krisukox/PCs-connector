#include "./ui_MainWindow.h"

#include <QCoreApplication>
#include <QMouseEvent>
#include "gui/GraphicsRectItem.h"
#include "gui/GraphicsScene.h"
#include "gui/MainWindow.h"

namespace
{
char** convertToArgv(QStringList commandArgs)
{
    int count = 0;
    char** argv = new char*[commandArgs.size()];

    for (auto t = commandArgs.begin(); t != commandArgs.end(); t++)
    {
        const auto& itemStr = (*t).toStdString();
        char* item = new char[itemStr.size() + 1];
        itemStr.copy(item, itemStr.size() + 1);
        item[itemStr.size()] = '\0';
        argv[count] = item;
        count++;
    }
    return argv;
}
} // namespace

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow{parent}, ui{new Ui::MainWindow}, app{std::make_unique<app_management::App>()}
{
    ui->setupUi(this);

    auto setContactPoints = [this](const std::pair<QPointF, QPointF>& contactPoints, const QPointF& diffPoint) {
        this->app->setContactPoints(
            {{static_cast<short>(contactPoints.first.x()), static_cast<short>(contactPoints.first.y())},
             {static_cast<short>(contactPoints.second.x()), static_cast<short>(contactPoints.second.y())}},
            {static_cast<short>(diffPoint.x()), static_cast<short>(diffPoint.y())});
    };
    auto scene = new GraphicsScene(0, 0, 598, 598, std::move(setContactPoints));

    ui->graphicsView->setScene(scene);

    GraphicsRectItem* item = new GraphicsRectItem(QRectF(0, 0, 136, 76));
    item->setBrush(QBrush(Qt::green));
    item->setFlags(QGraphicsItem::ItemIsMovable);

    GraphicsRectItem* item2 = new GraphicsRectItem(QRectF(0, 0, 196, 108));
    item2->setBrush(QBrush(Qt::blue));
    item2->setFlags(QGraphicsItem::ItemIsMovable);

    item->setCallback([item2]() { item2->setZValue(0); });
    item2->setCallback([item]() { item->setZValue(0); });

    item->setX(100);
    item->setY(100);
    item2->setZValue(1);
    item->setZValue(0);
    scene->addItem(item);
    scene->addItem(item2);

    QMouseEvent event(QEvent::GraphicsSceneMouseRelease, QPointF(), Qt::MouseButton::LeftButton, 0, 0);
    QCoreApplication::sendEvent(scene, &event);

    auto rect1 = item->rectPlaced();
    auto rect2 = item->rectPlaced();

    appThread =
        std::thread(&app_management::App::start, app.get(), qApp->arguments().size(), convertToArgv(qApp->arguments()));
}

MainWindow::~MainWindow()
{
    //    appThread.join();
    delete ui;
}
