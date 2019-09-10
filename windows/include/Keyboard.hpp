#pragma once

#include <Windows.h>
#include <array>
#include <cstddef>
#include <functional>
#include <memory>
#include <unordered_map>

using keyId = unsigned long long;
using Buffer = std::array<std::byte, 5>;

class Keyboard : std::enable_shared_from_this<Keyboard>
{
public:
    Keyboard(std::function<void(Buffer)>&&, std::function<void()>&&);
    Keyboard() = delete;

    void start();

private:
    std::function<void(Buffer)> pressedKeyCallback;
    std::function<void()> stopAppCallback;
    HHOOK keyboardHook;
    HHOOK mouseHook;
    bool isWinLPressed = false;
    bool isCtrlPressed = false;
    bool isShiftPressed = false;

    void changeState();
    void changeCtrl(Buffer asciiCode);
    void changeShift(Buffer asciiCode);
    bool hookState = true;
};
