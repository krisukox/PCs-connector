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
            {{static_cast<short>(contactPoints.first.x() * SCREEN_SIZE_MULTIPLIER),
              static_cast<short>(contactPoints.first.y() * SCREEN_SIZE_MULTIPLIER)},
             {static_cast<short>(contactPoints.second.x() * SCREEN_SIZE_MULTIPLIER),
              static_cast<short>(contactPoints.second.y() * SCREEN_SIZE_MULTIPLIER)}},
            {static_cast<short>(diffPoint.x() * SCREEN_SIZE_MULTIPLIER),
             static_cast<short>(diffPoint.y() * SCREEN_SIZE_MULTIPLIER)});
    };
    auto scene = new GraphicsScene(0, 0, 598, 598, std::move(setContactPoints));

    ui->graphicsView->setScene(scene);

    GraphicsRectItem* item = new GraphicsRectItem(
        QRectF(0, 0, MASTER_SCREEN_WIDTH / SCREEN_SIZE_MULTIPLIER, MASTER_SCREEN_HEIGHT / SCREEN_SIZE_MULTIPLIER),
        GraphicsRectItem::ScreenType::master);
    item->setBrush(QBrush(Qt::green));
    item->setFlags(QGraphicsItem::ItemIsMovable);

    GraphicsRectItem* item2 = new GraphicsRectItem(
        QRectF(0, 0, SLAVE_SCREEN_WIDTH / SCREEN_SIZE_MULTIPLIER, SLAVE_SCREEN_HEIGHT / SCREEN_SIZE_MULTIPLIER),
        GraphicsRectItem::ScreenType::slave);
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

    appThread =
        std::thread(&app_management::App::start, app.get(), qApp->arguments().size(), convertToArgv(qApp->arguments()));
}

MainWindow::~MainWindow()
{
    appThread.join();
    delete ui;
}
