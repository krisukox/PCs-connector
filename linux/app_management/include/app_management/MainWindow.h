#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define __stdcall

#include <QMainWindow>
#include <memory>
#include "App.hpp"

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
};
#endif // MAINWINDOW_H
