#include "app_management/Console.hpp"

namespace app_management
{
Console::Console()
{
    consoleHandle = GetStdHandle(STD_INPUT_HANDLE);
    GetConsoleMode(consoleHandle, &previousMode);
    SetConsoleMode(consoleHandle, previousMode & ENABLE_EXTENDED_FLAGS);
}

Console::~Console()
{
    SetConsoleMode(consoleHandle, previousMode);
}
} // namespace app_management
