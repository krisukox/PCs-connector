#include <iostream>
#include <stdlib.h>
#include "Windows.h"
#include "app_management/App.hpp"

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        char* address = new char[15];
        std::cin >> address;
        argv[1] = address;
        argc = 2;
    }
    try
    {
        app_management::App{argc, argv};
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }
    return 0;
}
