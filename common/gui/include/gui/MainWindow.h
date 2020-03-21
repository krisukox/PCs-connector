#pragma once

#include <QMainWindow>
#include <QThread>
#include <memory>
#include <thread>
#include "commons/IApp.hpp"

class Thread : public QThread
{
public:
    Thread(commons::IApp&, int argc, char** argv, commons::IApp::SetScreenResolution);
    Q_OBJECT
    void run() override;

    commons::IApp& app;
    int argc;
    char** argv;
    commons::IApp::SetScreenResolution setScreenResolution;
};

// namespace commons
//{
// class IApp;
//}

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

private:
    void addScreensToScene(const QSize&);

    std::thread appThread;
    Thread* thread;

    const QSize MASTER_SIZE;
    const QSize SLAVE_SIZE;
};
