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
};
