#include "key_management/TestKey.hpp"
#include <iostream>

namespace key_management
{
TestKey::TestKey()
{
    std::cout.setstate(std::ios_base::failbit);
}

TestKey::~TestKey()
{
    std::cout.clear();
}

void TestKey::handleEvent(std::byte keyId, std::byte) const
{
    std::cout.clear();
    std::cout << static_cast<uint8_t>(keyId) << std::endl;
    std::cout.setstate(std::ios_base::failbit);
}
} // namespace key_management
