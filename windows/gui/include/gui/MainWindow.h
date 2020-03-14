#pragma once

#include <QMainWindow>
#include <memory>
#include <thread>

#include "app_management/App.hpp"

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
    std::unique_ptr<app_management::App> app;

private:
    std::thread appThread;
    const unsigned SCREEN_SIZE_MULTIPLIER = 10;
    // HARDCODE
    const unsigned MASTER_SCREEN_WIDTH = 1920;
    const unsigned MASTER_SCREEN_HEIGHT = 1080;
    const unsigned SLAVE_SCREEN_WIDTH = 1366;
    const unsigned SLAVE_SCREEN_HEIGHT = 768;
    // HARDCODE END
};
