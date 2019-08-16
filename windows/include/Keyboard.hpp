#pragma once

#include <Windows.h>
#include <array>
#include <cstddef>
#include <functional>
#include <memory>
#include <unordered_map>

using keyId = unsigned long long;
using asciiCode = std::array<std::byte, 2>;

class Keyboard : std::enable_shared_from_this<Keyboard>
{
public:
    Keyboard(std::function<void(asciiCode)>&&, std::function<void()>&&);
    Keyboard() = delete;

private:
    std::function<void(asciiCode)> pressedKeyCallback;
    std::function<void()> stopAppCallback;
    HHOOK keyboardHook;
};
