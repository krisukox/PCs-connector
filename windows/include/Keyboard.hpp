#pragma once

#include <Windows.h>
#include <array>
#include <cstddef>
#include <functional>
#include <memory>
#include <unordered_map>
#include "internal_types/CommonTypes.hpp"

using keyId = unsigned long long;
using Buffer = std::array<std::byte, 5>;

class Keyboard : std::enable_shared_from_this<Keyboard>
{
public:
    Keyboard(std::function<void(internal_types::Event)>&&, std::function<void()>&&);
    Keyboard() = delete;

    void start();
    void changeState();

private:
    std::function<void(internal_types::Event)> pressedKeyCallback;
    std::function<void()> stopAppCallback;
    HHOOK keyboardHook;
    HHOOK mouseHook;
    bool isWinLPressed = false;
    bool isCtrlPressed = false;
    bool isShiftPressed = false;

    void changeCtrl(internal_types::KeyEvent);
    void changeShift(internal_types::KeyEvent);
    bool hookState = false;
};
