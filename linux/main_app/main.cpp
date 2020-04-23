#include <QApplication>
#include "gui/MainWindow.h"

#include "app_management/App.hpp"

int main(int argc, char* argv[])
{
    if (argc == 2 && !std::strcmp(argv[1], "test"))
    {
        app_management::App app{};
        app.test();
        while (true)
            ;
    }
    else if (argc == 1)
    {
        QApplication a(argc, argv);
        MainWindow w;
        w.show();
        return a.exec();
    }
    throw std::invalid_argument("not valid startup argument");
}
