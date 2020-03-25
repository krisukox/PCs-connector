#include "ui_MainWindow.h"

#include <QComboBox>
#include <QCoreApplication>
#include <QDebug>
#include <QGuiApplication>
#include <QLabel>
#include <QMouseEvent>
#include <QScreen>
#include <boost/asio.hpp>
#include "app_management/App.hpp"
#include "commons/CursorGuard.hpp"
#include "gui/GraphicsRectItem.h"
#include "gui/GraphicsScene.h"
#include "gui/MainWindow.h"
#include "internal_types/ScreenResolution.hpp"

namespace
{
const unsigned SCREEN_SIZE_MULTIPLIER = 10;

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

QSize getMasterSize()
{
    return qApp->screens().at(0)->size();
}

short upScale(const qreal& value)
{
    return static_cast<short>(value * SCREEN_SIZE_MULTIPLIER);
}

int downScale(const int& value)
{
    return value / SCREEN_SIZE_MULTIPLIER;
}
} // namespace

std::unique_ptr<commons::IApp> MainWindow::createAppPtr()
{
    auto screenGeometry = QGuiApplication::screens().at(0)->geometry();
    return std::make_unique<app_management::App>(
        std::make_shared<commons::CursorGuard>(screenGeometry.x(), screenGeometry.y()),
        [this](const internal_types::ScreenResolution& screenResolution) { emit messageSent(screenResolution); },
        internal_types::ScreenResolution{static_cast<std::uint16_t>(MASTER_SIZE.width()),
                                         static_cast<std::uint16_t>(MASTER_SIZE.height())});
}

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow{parent}, ui{new Ui::MainWindow}, MASTER_SIZE{getMasterSize()}, app{createAppPtr()}
{
    connect(this, SIGNAL(messageSent(ScreenResolutionMsg)), this, SLOT(handleScreenResolutionSet(ScreenResolutionMsg)));

    qRegisterMetaType<ScreenResolutionMsg>("ScreenResolutionMsg");
    ui->setupUi(this);
    connect(ui->connectButton, SIGNAL(released()), this, SLOT(handleConnectButton()));
    connect(ui->startButton, SIGNAL(released()), this, SLOT(handleStartButton()));

    auto setContactPoints = [this](const std::pair<QPointF, QPointF>& contactPoints, const QPointF& diffPoint) {
        this->app->setContactPoints(
            {{upScale(contactPoints.first.x()), upScale(contactPoints.first.y())},
             {upScale(contactPoints.second.x()), upScale(contactPoints.second.y())}},
            {upScale(diffPoint.x()), upScale(diffPoint.y())});
    };
    auto scene = new GraphicsScene(0, 0, 448, 448, std::move(setContactPoints));
    ui->graphicsView->setScene(scene);
    ui->infoLabel->setText("");

    fillAvailableMonitors();
}

void MainWindow::fillAvailableMonitors()
{
    QComboBox* availableMonitors = ui->availableMonitors;
    for (auto& screen : qApp->screens())
    {
        if (screen->model() == "")
        {
            availableMonitors->addItem("Buit-in screen");
        }
        else
        {
            availableMonitors->addItem(screen->model());
        }
    }
}

void MainWindow::addScreensToScene(const QSize& slaveSize)
{
    GraphicsRectItem* item = new GraphicsRectItem(
        QRectF(0, 0, downScale(MASTER_SIZE.width()), downScale(MASTER_SIZE.height())),
        GraphicsRectItem::ScreenType::master);
    item->setBrush(QBrush(Qt::green));
    item->setFlags(QGraphicsItem::ItemIsMovable);

    GraphicsRectItem* item2 = new GraphicsRectItem(
        QRectF(0, 0, downScale(slaveSize.width()), downScale(slaveSize.height())), GraphicsRectItem::ScreenType::slave);
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
    boost::asio::ip::address address;
    boost::system::error_code errorCode;
    if (qApp->arguments().size() == 2)
    {
        address = boost::asio::ip::make_address(convertToArgv(qApp->arguments())[1], errorCode);
    }
    else
    {
        address = boost::asio::ip::make_address(ui->inputIpAddress->text().toStdString(), errorCode);
    }
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

void MainWindow::handleStartButton()
{
    appThread =
        std::thread(&commons::IApp::listen, app.get(), qApp->arguments().size(), convertToArgv(qApp->arguments()));
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
