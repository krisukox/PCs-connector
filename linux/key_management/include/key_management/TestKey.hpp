#pragma once
#include <cstdint>
#include "key_management/IKey.hpp"

namespace key_management
{
class TestKey : public IKey
{
public:
    TestKey();
    ~TestKey() override;
    void handleEvent(std::uint16_t) override;
};
} // namespace key_management
