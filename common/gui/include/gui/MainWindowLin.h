#pragma once

#include <QMainWindow>
#include <memory>
#include <thread>

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

private:
    std::thread appThread;

    const QSize MASTER_SIZE;
    const QSize SLAVE_SIZE;
};
