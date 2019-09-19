#pragma once

#include <Windows.h>

namespace app_management
{
class Console
{
public:
    Console();
    ~Console();

private:
    DWORD previousMode;
    HANDLE consoleHandle;
};
} // namespace app_management
