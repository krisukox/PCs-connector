#include <cstddef>
#include <iostream>
#include "app_management/App.hpp"

int main(int argc, char* argv[])
{
    try
    {
        app_management::App{argc, argv};
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }
    //    auto cos = std::byte{static_cast<std::uint8_t>(-128)};
    //    if (cos == std::byte{0b10000000}) std::cout << "TAK" << std::endl;
    //    int cos1 = std::to_integer<std::int8_t>(cos);
    //    std::cout << cos1 << std::endl;
    return 0;
}
