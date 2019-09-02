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

void TestKey::onEvent(internal_types::KeyEvent keyEvent) const
{
    std::cerr << ">>>  " << static_cast<uint16_t>(keyEvent.keyCode) << std::endl;
    std::cout.clear();
    std::cout << keyEvent.keyCode << std::endl;
    std::cout.setstate(std::ios_base::failbit);
}
} // namespace key_management
