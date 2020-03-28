#pragma once

#include <QMainWindow>
#include <memory>
#include <thread>
#include "gui/ScreenIndicator.h"
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
    void handleIdetifyScreensButton();

signals:
    void messageSent(const ScreenResolutionMsg&);

private:
    std::unique_ptr<commons::IApp> createAppPtr();
    void fillAvailableMonitors();
    void showIndicators();
    void addScreensToScene(const QSize&);

    const QSize MASTER_SIZE;
    std::unique_ptr<commons::IApp> app;
    std::unique_ptr<MainWindow> mainWindow;
    std::unique_ptr<Indicator> indicator1;
    std::unique_ptr<Indicator> indicator2;
    static unsigned index;

    std::thread appThread;
    std::vector<std::unique_ptr<Indicator>> indicators;
    QTimer* timer;
};
