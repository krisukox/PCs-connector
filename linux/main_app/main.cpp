#include <QApplication>
#include "gui/MainWindow.h"

#include <thread>
#include "app_management/App.hpp"
#include "internal_types/ScreenResolution.hpp"

int main(int argc, char* argv[])
{
    if (argc == 4 && !std::strcmp(argv[1], "test"))
    {
        auto app = std::make_unique<app_management::App>();
        app->test({static_cast<uint16_t>(std::atoi(argv[2])), static_cast<uint16_t>(std::atoi(argv[3]))});
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
