#pragma once

#include <unordered_map>
#include "IDeserializer.hpp"
#include "KeyEvent.hpp"
#include "server_app/ServerAppTypes.hpp"

namespace internal_types
{
class Deserializer : public IDeserializer
{
public:
    Deserializer(Display*);

    ~Deserializer() override = default;
    KeyEvent decode(const server_app::Buffer&) const override;

private:
    KeyCode decodeKeyCode(const std::byte) const;
    bool decodeKeyState(const std::byte) const;

    Display* display;
    const std::unordered_map<std::byte, KeyCode> translationTabel;
};
} // namespace internal_types
