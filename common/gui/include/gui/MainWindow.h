#pragma once

#include <QMainWindow>
#include <QThread>
#include <memory>
#include <thread>
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

private slots:
    void handleConnectButton();
    void handleStartButton();
    void handleScreenResolutionSet(const ScreenResolutionMsg&);

signals:
    void messageSent(const ScreenResolutionMsg&);

private:
    std::unique_ptr<commons::IApp> createAppPtr();

    void addScreensToScene(const QSize&);

    std::thread appThread;
    std::unique_ptr<commons::IApp> app;

    const QSize MASTER_SIZE;
    const QSize SLAVE_SIZE;
};
