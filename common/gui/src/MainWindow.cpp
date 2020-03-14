#include "./ui_MainWindow.h"

#include <QCoreApplication>
#include <QMouseEvent>
#include <QScreen>
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

const unsigned SCREEN_SIZE_MULTIPLIER = 10;

const int SCREEN_WIDTH_1 = 1920;
const int SCREEN_HEIGHT_1 = 1080;
const int SCREEN_WIDTH_2 = 1366;
const int SCREEN_HEIGHT_2 = 768;

QSize getMasterSize()
{
    return qApp->screens().at(0)->size();
}

QSize getSlaveSize()
{
    if (qApp->screens().at(0)->size().width() == SCREEN_WIDTH_1)
    {
        return QSize{SCREEN_WIDTH_2, SCREEN_HEIGHT_2};
    }
    else
    {
        return QSize{SCREEN_WIDTH_1, SCREEN_HEIGHT_1};
    }
}
} // namespace

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow{parent}
    , ui{new Ui::MainWindow}
    , app{std::make_unique<app_management::App>()}
    , MASTER_SIZE{getMasterSize()}
    , SLAVE_SIZE{getSlaveSize()}
{
    ui->setupUi(this);

    auto setContactPoints = [this](const std::pair<QPointF, QPointF>& contactPoints, const QPointF& diffPoint) {};
    auto scene = new GraphicsScene(0, 0, 598, 598, std::move(setContactPoints));

    ui->graphicsView->setScene(scene);

    GraphicsRectItem* item = new GraphicsRectItem(
        QRectF(0, 0, MASTER_SIZE.width() / SCREEN_SIZE_MULTIPLIER, MASTER_SIZE.height() / SCREEN_SIZE_MULTIPLIER),
        GraphicsRectItem::ScreenType::master);
    item->setBrush(QBrush(Qt::green));
    item->setFlags(QGraphicsItem::ItemIsMovable);

    GraphicsRectItem* item2 = new GraphicsRectItem(
        QRectF(0, 0, SLAVE_SIZE.width() / SCREEN_SIZE_MULTIPLIER, SLAVE_SIZE.height() / SCREEN_SIZE_MULTIPLIER),
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
