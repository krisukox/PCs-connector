#include <array>
#include <cstddef>
#include <iostream>
#include "app_management/App.hpp"

int main(int argc, char* argv[])
{
    try
    {
        app_management::App app{argc, argv};
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
