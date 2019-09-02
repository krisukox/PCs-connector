#include <array>
#include <cstddef>
#include <iostream>
#include "app_management/App.hpp"

#include <cstddef>

int main(int argc, char* argv[])
{
    //    auto d1 = std::move(d);

    //    std::byte first{0b01001000};
    //    std::byte second{0b00100010};

    //    uint16_t aggregate = static_cast<uint16_t>((static_cast<uint8_t>(first) << 8) +
    //    static_cast<uint8_t>(second)); std::cout << aggregate << std::endl;

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
