#pragma once

#include <Windows.h>
#include <array>
#include <cstddef>
#include <functional>
#include <memory>
#include <unordered_map>
//#include "Types.hpp"
#include "internal_types/MouseEvent.hpp"

class Mouse : std::enable_shared_from_this<Mouse>
{
public:
    Mouse(std::function<void(internal_types::MouseEvent)>&&, std::function<void()>&&);
    Mouse() = delete;

    void start();

private:
    std::function<void(internal_types::MouseEvent)> pressedKeyCallback;
    std::function<void()> stopAppCallback;
    HHOOK mouseHook;
};
