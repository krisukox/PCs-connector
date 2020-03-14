//#include <iostream>
//#include <stdlib.h>
//#include "Windows.h"
//#include "app_management/App.hpp"
#include <QApplication>
#include "gui/MainWindow.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();

    //    try
    //    {
    //        app_management::App{argc, argv};
    //    }
    //    catch (std::exception& e)
    //    {
    //        std::cerr << "Exception: " << e.what() << "\n";
    //    }
    //    return 0;
}
