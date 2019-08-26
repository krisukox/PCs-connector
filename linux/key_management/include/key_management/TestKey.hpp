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

    void handleEvent(std::byte, std::byte) const override;
};
} // namespace key_management
