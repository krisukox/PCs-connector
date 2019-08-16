#pragma once

#include <functional>
#include <unordered_map>

using keyId = unsigned long long;
using asciiCode = unsigned;

class Keyboard
{
public:
    Keyboard(std::function<void(unsigned)>&&, std::function<void()>&&);
    Keyboard() = delete;

private:
    void registerKeys();
    void unregiserKeys();
    std::unordered_map<keyId, asciiCode> prepareKeys();

    const std::unordered_map<keyId, asciiCode> keys = prepareKeys();
    bool registeredKeys = false;
    std::function<void(unsigned)> pressedKeyCallback;
    std::function<void()> stopAppCallback;
};
