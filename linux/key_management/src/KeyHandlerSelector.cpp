#include "key_management/KeyHandlerSelector.hpp"
#include <cstring>
#include <iostream>
#include "key_management/FakeKey.hpp"
#include "key_management/TestKey.hpp"

namespace key_management
{
KeyHandlerSelector::KeyHandlerSelector(Display* display_) : display{display_} {}

std::shared_ptr<IKey> KeyHandlerSelector::select(int argc, char* argv[])
{
    if (argc == 2 && !std::strcmp(argv[1], "test"))
    {
        std::clog << "test mode started" << std::endl;
        return std::make_shared<key_management::TestKey>();
    }
    else if (argc == 1)
    {
        std::clog << "release mode started" << std::endl;
        return std::make_shared<key_management::FakeKey>(display);
    }
    throw std::invalid_argument("not valid startup argument");
}
} // namespace key_management
