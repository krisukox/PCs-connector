#include "ui_MainWindow.h"

#include <QCoreApplication>
#include <QDebug>
#include <QGuiApplication>
#include <QLabel>
#include <QLineEdit>
#include <QMouseEvent>
#include <QScreen>
#include <boost/asio.hpp>
#include <iostream>
#include "app_management/App.hpp"
#include "commons/CursorGuard.hpp"
//#include "commons/IApp.hpp"
#include "gui/GraphicsRectItem.h"
#include "gui/GraphicsScene.h"
#include "gui/MainWindow.h"
#include "gui/ScreenResolutionMsg.h"
#include "internal_types/ScreenResolution.hpp"

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
    , msgSender{new MsgSender()}
    , app{createAppPtr()}
    , MASTER_SIZE{getMasterSize()}
    , SLAVE_SIZE{getSlaveSize()}
{
    connect(
        msgSender,
        SIGNAL(messageSent(ScreenResolutionMsg)),
        this,
        SLOT(handleScreenResolutionSet(ScreenResolutionMsg)));

    ui->setupUi(this);
    connect(ui->connectButton, SIGNAL(released()), this, SLOT(handleConnectButton()));
    connect(ui->startButton, SIGNAL(released()), this, SLOT(handleStartButton()));

    auto setContactPoints = [this](const std::pair<QPointF, QPointF>& contactPoints, const QPointF& diffPoint) {
        this->app->setContactPoints(
            {{static_cast<short>(contactPoints.first.x() * SCREEN_SIZE_MULTIPLIER),
              static_cast<short>(contactPoints.first.y() * SCREEN_SIZE_MULTIPLIER)},
             {static_cast<short>(contactPoints.second.x() * SCREEN_SIZE_MULTIPLIER),
              static_cast<short>(contactPoints.second.y() * SCREEN_SIZE_MULTIPLIER)}},
            {static_cast<short>(diffPoint.x() * SCREEN_SIZE_MULTIPLIER),
             static_cast<short>(diffPoint.y() * SCREEN_SIZE_MULTIPLIER)});
    };
    auto scene = new GraphicsScene(0, 0, 448, 448, std::move(setContactPoints));
    ui->graphicsView->setScene(scene);

    ui->infoLabel->setText("");
}

std::unique_ptr<commons::IApp> MainWindow::createAppPtr()
{
    auto screenGeometry = QGuiApplication::screens().at(0)->geometry();
    return std::make_unique<app_management::App>(
        std::make_shared<commons::CursorGuard>(screenGeometry.x(), screenGeometry.y()),
        [this](const internal_types::ScreenResolution& screenResolution) { msgSender->send(screenResolution); });
}

void MainWindow::addScreensToScene(const QSize& slaveSize)
{
    auto masterSize = getMasterSize();

    GraphicsRectItem* item = new GraphicsRectItem(
        QRectF(0, 0, masterSize.width() / SCREEN_SIZE_MULTIPLIER, masterSize.height() / SCREEN_SIZE_MULTIPLIER),
        GraphicsRectItem::ScreenType::master);
    item->setBrush(QBrush(Qt::green));
    item->setFlags(QGraphicsItem::ItemIsMovable);

    GraphicsRectItem* item2 = new GraphicsRectItem(
        QRectF(0, 0, slaveSize.width() / SCREEN_SIZE_MULTIPLIER, slaveSize.height() / SCREEN_SIZE_MULTIPLIER),
        GraphicsRectItem::ScreenType::slave);
    item2->setBrush(QBrush(Qt::blue));
    item2->setFlags(QGraphicsItem::ItemIsMovable);

    item->setCallback([item2]() { item2->setZValue(0); });
    item2->setCallback([item]() { item->setZValue(0); });

    item->setX(100);
    item->setY(100);
    item2->setZValue(1);
    item2->setY(160);
    item->setZValue(0);

    GraphicsScene* scene = dynamic_cast<GraphicsScene*>(ui->graphicsView->scene());
    if (scene)
    {
        scene->addItem(item);
        scene->addItem(item2);

        QMouseEvent event(QEvent::GraphicsSceneMouseRelease, QPointF(), Qt::MouseButton::LeftButton, 0, 0);
        QCoreApplication::sendEvent(scene, &event);
    }
}

void MainWindow::handleConnectButton()
{
    if (qApp->arguments().size() == 2)
    {
        auto address = boost::asio::ip::make_address(convertToArgv(qApp->arguments())[1]);
        appThread = std::thread(&commons::IApp::connect, app.get(), address);
    }
    else
    {
        boost::system::error_code errorCode;
        auto address = boost::asio::ip::make_address(ui->inputIpAddress->text().toStdString(), errorCode);
        if (!errorCode.failed())
        {
            appThread = std::thread(&commons::IApp::connect, app.get(), address);
            ui->infoLabel->setText("");
        }
        else
        {
            ui->infoLabel->setText("Ip address not correct");
        }
    }
}

void MainWindow::handleStartButton()
{
    auto setSlaveScreenResolution = [this](const internal_types::ScreenResolution& screenResolution) {
        msgSender->send(screenResolution);
    };
    appThread = std::thread(
        &commons::IApp::listen,
        app.get(),
        qApp->arguments().size(),
        convertToArgv(qApp->arguments()),
        internal_types::ScreenResolution{static_cast<std::uint16_t>(MASTER_SIZE.width()),
                                         static_cast<std::uint16_t>(MASTER_SIZE.height())});
}

void MainWindow::handleScreenResolutionSet(const ScreenResolutionMsg& screenResolutionMsg)
{
    addScreensToScene(QSize(screenResolutionMsg.width, screenResolutionMsg.height));
}

MainWindow::~MainWindow()
{
    appThread.join();
    delete ui;
}
