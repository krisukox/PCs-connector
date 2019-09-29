#include "event_consumer/TestKeyboardReceiver.hpp"
#include <iostream>

namespace event_consumer
{
TestKeyboardReceiver::TestKeyboardReceiver()
{
    std::cout.setstate(std::ios_base::failbit);
}

TestKeyboardReceiver::~TestKeyboardReceiver()
{
    std::cout.clear();
}

void TestKeyboardReceiver::onEvent(const internal_types::KeyEvent& keyEvent) const
{
    std::cout.clear();
    std::cout << keyEvent.keyCode << std::endl;
    std::cout.setstate(std::ios_base::failbit);
}
} // namespace event_consumer
