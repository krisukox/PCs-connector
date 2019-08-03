#pragma once
#include <cstdint>
#include "../include/IKey.hpp"

class TestKey : public IKey
{
public:
    TestKey();
    ~TestKey() override;
    void handleEvent(std::uint16_t) override;
};
