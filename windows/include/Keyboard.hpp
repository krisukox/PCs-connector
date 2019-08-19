#pragma once

#include <Windows.h>
#include <array>
#include <cstddef>
#include <functional>
#include <memory>
#include <unordered_map>

using keyId = unsigned long long;
using AsciiCode = std::array<std::byte, 2>;

class Keyboard : std::enable_shared_from_this<Keyboard>
{
public:
    Keyboard(std::function<void(AsciiCode)>&&, std::function<void()>&&);
    Keyboard() = delete;

    void start();

private:
    std::function<void(AsciiCode)> pressedKeyCallback;
    std::function<void()> stopAppCallback;
    HHOOK keyboardHook;
    bool isWinLPressed = false;
    bool isCtrlPressed = false;
    bool isShiftPressed = false;

    void changeState();
    void changeCtrl(AsciiCode asciiCode);
    void changeShift(AsciiCode asciiCode);
    bool hookState = true;
};
