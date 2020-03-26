#include <QApplication>
#include "gui/MainWindow.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    //    MainWindow w1;
    //    w1.show();
    return a.exec();
}
