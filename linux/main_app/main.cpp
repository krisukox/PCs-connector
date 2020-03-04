#include <QApplication>
#include "../app_management/include/app_management/MainWindow.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
