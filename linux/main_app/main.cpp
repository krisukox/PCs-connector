#include <QApplication>
#include <QtWidgets>
#include "../app_management/include/app_management/MainWindow.h"

//#include <X11/Xlib.h>
//#include <X11/extensions/XTest.h>
#include <iostream>
//#include "app_management/App.hpp"

//#include "./ui_MainWindow.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();

    //    try
    //    {
    //        app_management::App app{argc, argv};
    //    }
    //    catch (std::exception& e)
    //    {
    //        std::cerr << "Exception: " << e.what() << "\n";
    //    }

    return 0;
}
