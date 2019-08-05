#pragma once

#include <memory>

namespace key_management
{
class IKey;

class KeyHandlerSelector
{
public:
    KeyHandlerSelector() = default;

    std::shared_ptr<IKey> select(int, char* []);
};
} // namespace key_management
