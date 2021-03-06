#include "ui_MainWindow.h"

#include <QComboBox>
#include <QCoreApplication>
#include <QDebug>
#include <QGuiApplication>
#include <QLabel>
#include <QMouseEvent>
#include <QScreen>
#include <QThread>
#include <QTimer>
#include <boost/asio.hpp>
#include "app_management/App.hpp"
#include "commons/CursorGuard.hpp"
#include "gui/CursorManagement.h"
#include "gui/GraphicsRectItem.h"
#include "gui/GraphicsScene.h"
#include "gui/MainWindow.h"
#include "internal_types/Point.hpp"
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

void upScale(QPointF& value)
{
    value.setX(value.x() * SCREEN_SIZE_MULTIPLIER);
    value.setY(value.y() * SCREEN_SIZE_MULTIPLIER);
}

void upScale(std::pair<QPointF, QPointF>& value)
{
    value.first.setX(value.first.x() * SCREEN_SIZE_MULTIPLIER);
    value.first.setY(value.first.y() * SCREEN_SIZE_MULTIPLIER);
    value.second.setX(value.second.x() * SCREEN_SIZE_MULTIPLIER);
    value.second.setY(value.second.y() * SCREEN_SIZE_MULTIPLIER);
}

qreal downScale(const int& value)
{
    return static_cast<qreal>(value) / SCREEN_SIZE_MULTIPLIER;
}

internal_types::Point toInternalType(const QPointF& point)
{
    return {static_cast<short>(point.x()), static_cast<short>(point.y())};
}

internal_types::ScreenResolution toInternalType(const QSize& resolution)
{
    return {static_cast<uint16_t>(resolution.width()), static_cast<uint16_t>(resolution.height())};
}

void alignContactPoints(std::pair<QPointF, QPointF>& contactPoints, const QRect& screenRect)
{
    contactPoints.first += screenRect.topLeft();
    contactPoints.second += screenRect.topLeft();
}

QPointF getDiffPointForSend(const QPointF& diffPoint, const QRect& screenGeometry)
{
    return diffPoint - screenGeometry.topLeft();
}
} // namespace

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow{parent}, ui{new Ui::MainWindow}, app{std::make_unique<app_management::App>()}
{
    connect(this, SIGNAL(messageSent(ScreenResolutionMsg)), this, SLOT(handleScreenResolutionSet(ScreenResolutionMsg)));

    qRegisterMetaType<ScreenResolutionMsg>("ScreenResolutionMsg");
    ui->setupUi(this);
    timer = new QTimer(this);

    GraphicsScene::SetContactPoints setContactPoints = [this](
                                                           std::pair<QPointF, QPointF>& contactPoints,
                                                           QPointF& diffPoint) {
        upScale(contactPoints);
        upScale(diffPoint);

        QComboBox* availableMonitors = ui->availableMonitors;
        auto screenGeometry = qApp->screens().at(availableMonitors->currentIndex())->geometry();

        QPointF diffPointForSend = getDiffPointForSend(diffPoint, screenGeometry);

        alignContactPoints(contactPoints, screenGeometry);
        auto diffPointForReceive = screenGeometry.topLeft();
        this->app->setTransformationPoints({{toInternalType(contactPoints.first), toInternalType(contactPoints.second)},
                                            toInternalType(diffPointForSend),
                                            toInternalType(diffPointForReceive)});
    };
    auto scene = new GraphicsScene(0, 0, 448, 448, std::move(setContactPoints));
    ui->graphicsView->setScene(scene);
    ui->infoLabel->setText("");
    ui->notificationLabel->setText("");
    ui->serverIsRunningLabel->setText("");
    fillAvailableMonitors();

    timer->callOnTimeout([this]() {
        indicators.clear();
        timer->stop();
    });

    connect(ui->connectButton, SIGNAL(released()), this, SLOT(handleConnectButton()));
    connect(ui->startButton, SIGNAL(released()), this, SLOT(handleStartButton()));
    connect(ui->identifyScreens, SIGNAL(released()), this, SLOT(handleIdetifyScreensButton()));
    connect(ui->availableMonitors, SIGNAL(currentIndexChanged(int)), this, SLOT(borderScreenChanged(int)));
}

void MainWindow::fillAvailableMonitors()
{
    QComboBox* availableMonitors = ui->availableMonitors;
    auto size = qApp->screens().size();
    for (int i = 0; i < size; i++)
    {
        availableMonitors->addItem(QString::number(i + 1), i + 1);
    }
}

void MainWindow::showIndicators()
{
    QComboBox* availableMonitors = ui->availableMonitors;

    auto size = qApp->screens().size();

    indicators.clear();
    for (int i = 0; i < size; i++)
    {
        auto indicator = std::make_unique<Indicator>();
        if (i + 1 == availableMonitors->currentData().toInt())
        {
            indicator->setAppearance(i + 1, Qt::red);
        }
        else
        {
            indicator->setAppearance(i + 1, Qt::black);
        }
        indicator->setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
        indicator->setFocusPolicy(Qt::FocusPolicy::NoFocus);
        indicator->setWindowOpacity(0.7);
        indicator->move(qApp->screens().at(i)->geometry().topLeft() + QPoint(100, 100));
        indicator->show();
        indicators.push_back(std::move(indicator));
    }
    timer->start(5000);
}

void MainWindow::addScreensToScene(const QSize& slaveSize)
{
    QComboBox* availableMonitors = ui->availableMonitors;
    auto masterSize = qApp->screens().at(availableMonitors->currentIndex())->size();
    GraphicsRectItem* item = new GraphicsRectItem(
        QRectF(0, 0, downScale(masterSize.width()), downScale(masterSize.height())),
        GraphicsRectItem::ScreenType::master);
    item->setBrush(QBrush(Qt::red));
    item->setFlags(QGraphicsItem::ItemIsMovable);

    GraphicsRectItem* item2 = new GraphicsRectItem(
        QRectF(0, 0, downScale(slaveSize.width()), downScale(slaveSize.height())), GraphicsRectItem::ScreenType::slave);
    item2->setBrush(QBrush(Qt::cyan));
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
    CursorManagement::initialize();
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
        QComboBox* availableMonitors = ui->availableMonitors;
        try
        {
            app->connect(
                address,
                toInternalType(qApp->screens().at(availableMonitors->currentIndex())->size()),
                [this](const internal_types::ScreenResolution screenResolution) {
                    emit messageSent(screenResolution);
                });
            ui->infoLabel->setText("");
            ui->notificationLabel->setText("");
        }
        catch (...)
        {
            ui->notificationLabel->setText("Client is not implemented yet");
        }
    }
    else
    {
        ui->infoLabel->setText("Ip address not correct");
    }
}

void MainWindow::handleStartButton()
{
    QComboBox* availableMonitors = ui->availableMonitors;
    CursorManagement::initialize();
    try
    {
        app->listen(
            toInternalType(qApp->screens().at(availableMonitors->currentIndex())->size()),
            [this](const internal_types::ScreenResolution screenResolution) { emit messageSent(screenResolution); });
        ui->notificationLabel->setText("");
        ui->serverIsRunningLabel->setText("Server is running");
    }
    catch (...)
    {
        ui->notificationLabel->setText("Server is not implemented yet");
    }
}

void MainWindow::handleScreenResolutionSet(const ScreenResolutionMsg& screenResolutionMsg)
{
    addScreensToScene(QSize(screenResolutionMsg.width, screenResolutionMsg.height));
}

void MainWindow::handleIdetifyScreensButton()
{
    showIndicators();
}

void MainWindow::borderScreenChanged(const int&)
{
    GraphicsScene* scene = dynamic_cast<GraphicsScene*>(ui->graphicsView->scene());
    if (scene)
    {
        QMouseEvent event(QEvent::GraphicsSceneMouseRelease, QPointF(), Qt::MouseButton::LeftButton, 0, 0);
        QCoreApplication::sendEvent(scene, &event);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
