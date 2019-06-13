#include "Keyboard.hpp"

#include <Windows.h>


Keyboard::Keyboard(std::function<void(unsigned)> _callback):callback(_callback)
{
    RegisterHotKey(nullptr, 100, MOD_CONTROL | MOD_SHIFT, 'A');
    RegisterHotKey(nullptr, 200, MOD_CONTROL | MOD_SHIFT, 'Q');

    registerKeys();

    MSG msg;
    while(GetMessage(&msg, nullptr, 0, 0))
    {
        PeekMessage(&msg, nullptr, 0, 0, 0x0001);

        if(msg.message == WM_HOTKEY)
        {
            auto it = keys.find(msg.wParam);
            if(it != keys.end())
            {
                callback(it->second);
            }
            else if(msg.wParam == 100)
            {
                if(registeredKeys)
                    unregiserKeys();
                else
                    registerKeys();
            }
            else if(msg.wParam == 200)
            {
                return;
            }
        }
    }
}

void Keyboard::registerKeys()
{
    for(auto key : keys)
    {
        RegisterHotKey(nullptr, static_cast<int>(key.first), MOD_NOREPEAT, key.second);
    }
    registeredKeys = true;
}

void Keyboard::unregiserKeys()
{
    for(auto key : keys)
    {
        UnregisterHotKey(nullptr, static_cast<int>(key.first));
    }
    registeredKeys = false;
}

std::unordered_map<keyId, asciiCode> Keyboard::prepareKeys()
{
    std::unordered_map<keyId, asciiCode> keys;
    for(unsigned i = 0;i < 26;i++)
    {
        keys.insert(std::make_pair(i, i+65));
    }
    return keys;
}
