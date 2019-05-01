#pragma comment(lib, "ws2_32.lib")

#include "Client.hpp"

#include <stdexcept>
#include <iostream>



int main()
{
    try {
        Client client{};
    } catch (std::runtime_error e) {
        std::cout<<e.what();
    }
}
