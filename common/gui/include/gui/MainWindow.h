#pragma once

#include <QMainWindow>
#include <QThread>
#include <memory>
#include <thread>
#include "commons/IApp.hpp"
#include "gui/MsgSender.h"
#include "gui/ScreenResolutionMsg.h"

namespace commons
{
class IApp;
}

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    Ui::MainWindow* ui;
    std::unique_ptr<commons::IApp> app;

private slots:
    void handleConnectButton();
    void handleStartButton();
    void handleScreenResolutionSet(const ScreenResolutionMsg&);

private:
    void addScreensToScene(const QSize&);

    std::thread appThread;
    MsgSender* msgSender;

    const QSize MASTER_SIZE;
    const QSize SLAVE_SIZE;
};
