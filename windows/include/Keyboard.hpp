#pragma once

#include <unordered_map>
#include <functional>


using keyId = unsigned;
using asciiCode = unsigned;

class Keyboard
{
public:
    Keyboard(std::function<void(unsigned)>);
    Keyboard() = delete;

private:
    void registerKeys();
    void unregiserKeys();
    std::unordered_map<keyId, asciiCode> prepareKeys();

    const std::unordered_map<keyId, asciiCode> keys = prepareKeys();
    bool registeredKeys = false;
    std::function<void(unsigned)> callback;
};
