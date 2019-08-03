#include "../include/TestKey.hpp"
#include <iostream>

TestKey::TestKey()
{
    //    std::cout.setstate(std::ios_base::failbit);
}

TestKey::~TestKey()
{
    //    std::cout.clear();
}

void TestKey::handleEvent(std::uint16_t keyId) // handle event
{
    //    std::cout.clear();
    std::cout << keyId << std::endl;
    //    std::cout.setstate(std::ios_base::failbit);
}
