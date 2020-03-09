#include <QApplication>
#include "../gui/include/gui/MainWindow.h"
//#include "gui/

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
