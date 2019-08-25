#pragma once

#include <Windows.h>
#include <array>
#include <cstddef>
#include <functional>
#include <memory>
#include <unordered_map>
#include "Types.hpp"

class Mouse : std::enable_shared_from_this<Mouse>
{
public:
    Mouse(std::function<void(Buffer)>&&, std::function<void()>&&);
    Mouse() = delete;

    void start();

private:
    std::function<void(Buffer)> pressedKeyCallback;
    std::function<void()> stopAppCallback;
    HHOOK mouseHook;
};
