#include "internal_types/Deserializer.hpp"
#include <X11/keysym.h>
#include <cstddef>
#include "internal_types/SerializedValues.hpp"

namespace
{
constexpr std::uint16_t WIN_LINUX_FUNCTION_KEYCODE_DIFF{0xFF4E};
constexpr std::uint16_t WIN_LINUX_ARROWS_KEYCODE_DIFF{0xFF2C};
constexpr std::uint16_t WIN_LINUX_NUMPAD_KEYCODE_DIFF{0xFF50};
constexpr std::uint8_t WIN_LINUX_KEYCODE_DIFF_1{0x80};
constexpr std::uint8_t WIN_LINUX_KEYCODE_DIFF_2{0x90};
constexpr std::uint16_t WIN_LINUX_KEYCODE_DIFF_3{0xFF40};

constexpr std::byte WIN_A{0x41};
constexpr std::byte WIN_Z{0x5A};

constexpr std::byte WIN_0{0x30};
constexpr std::byte WIN_9{0x39};

constexpr std::byte WIN_F1{0x70};
constexpr std::byte WIN_F12{0x7B};

constexpr std::byte WIN_LSHIFT{0xA0};
constexpr std::byte WIN_RSHIFT{0xA1};
constexpr std::byte WIN_LCONTROL{0xA2};
constexpr std::byte WIN_RCONTROL{0xA3};
constexpr std::byte WIN_LMENU{0xA4};
constexpr std::byte WIN_RMENU{0xA5};
constexpr std::byte WIN_Menu{0x5D};
constexpr std::byte WIN_LWIN{0x5B};
constexpr std::byte WIN_space{0x20};

constexpr std::byte WIN_TAB{0x09};
constexpr std::byte WIN_GRAVE{0xC0};
constexpr std::byte WIN_RETURN{0x0D};
constexpr std::byte WIN_OEM_EQUAL{0xBB};
constexpr std::byte WIN_BACKSPACE{0x08};

constexpr std::byte WIN_BRACKETLEFT{0xDB};
constexpr std::byte WIN_BRACKETRIGHT{0xDD};

constexpr std::byte WIN_COMMA{0xBC};
constexpr std::byte WIN_SLASH{0xBF};

constexpr std::byte WIN_SEMICOLON{0xBA};
constexpr std::byte WIN_APOSTROPHE{0xDE};
constexpr std::byte WIN_CAPS_LOCK{0x14};
constexpr std::byte WIN_ESCAPE{0x1B};

constexpr std::byte WIN_Page_Up{0x21};
constexpr std::byte WIN_Page_Down{0x22};
constexpr std::byte WIN_End{0x23};
constexpr std::byte WIN_HOME{0x24};

constexpr std::byte WIN_Print{0x2C};
constexpr std::byte WIN_Insert{0x2D};
constexpr std::byte WIN_Delete{0x2E};

constexpr std::byte WIN_Scroll_Lock{0x2D};
constexpr std::byte WIN_Pause{0x13};

constexpr std::byte WIN_Left{0x25};
constexpr std::byte WIN_Down{0x28};

constexpr std::byte WIN_NUMPAD0{0x60};
constexpr std::byte WIN_NUMPAD9{0x69};

constexpr std::byte WIN_MULTIPLY{0x6A};
constexpr std::byte WIN_DIVIDE{0x6F};

constexpr std::byte WIN_Num_Lock{0x90};

short toShort(const std::byte lv, const std::byte rv)
{
    return static_cast<short>((std::to_integer<uint8_t>(lv) << 8) + std::to_integer<uint8_t>(rv));
}

std::uint16_t toUInt(const std::byte lv, const std::byte rv)
{
    return (std::to_integer<uint8_t>(lv) << 8) + std::to_integer<uint8_t>(rv);
}

constexpr auto leftButtonPressed{std::to_integer<std::uint8_t>(std::byte{0b00000001})};
constexpr auto leftButtonUnpressed{std::to_integer<std::uint8_t>(std::byte{0b00000010})};
constexpr auto rightButtonPressed{std::to_integer<std::uint8_t>(std::byte{0b00000100})};
constexpr auto rightButtonUnpressed{std::to_integer<std::uint8_t>(std::byte{0b00001000})};
constexpr auto middleButtonPressed{std::to_integer<std::uint8_t>(std::byte{0b00010000})};
constexpr auto middleButtonUnpressed{std::to_integer<std::uint8_t>(std::byte{0b00100000})};

constexpr std::byte screenResolutionByte{0b00100000};
} // namespace

namespace internal_types
{
Deserializer::Deserializer(Display* display_)
    : display{display_}
    , translationTabel{
          {WIN_LSHIFT, XKeysymToKeycode(display, XK_Shift_L)},
          {WIN_RSHIFT, XKeysymToKeycode(display, XK_Shift_R)},
          {WIN_LCONTROL, XKeysymToKeycode(display, XK_Control_L)},
          {WIN_RCONTROL, XKeysymToKeycode(display, XK_Control_R)},
          {WIN_LMENU, XKeysymToKeycode(display, XK_Alt_L)},
          {WIN_RMENU, XKeysymToKeycode(display, XK_ISO_Level3_Shift)},
          {WIN_Menu, XKeysymToKeycode(display, XK_Menu)},
          {WIN_LWIN, XKeysymToKeycode(display, XK_Super_L)},
          {WIN_space, XKeysymToKeycode(display, XK_space)},
          {WIN_TAB, XKeysymToKeycode(display, XK_Tab)},
          {WIN_GRAVE, XKeysymToKeycode(display, XK_grave)},
          {WIN_RETURN, XKeysymToKeycode(display, XK_Return)},
          {WIN_OEM_EQUAL, XKeysymToKeycode(display, XK_equal)},
          {WIN_BACKSPACE, XKeysymToKeycode(display, XK_BackSpace)},
          {WIN_SEMICOLON, XKeysymToKeycode(display, XK_semicolon)},
          {WIN_APOSTROPHE, XKeysymToKeycode(display, XK_apostrophe)},
          {WIN_CAPS_LOCK, XKeysymToKeycode(display, XK_Caps_Lock)},
          {WIN_ESCAPE, XKeysymToKeycode(display, XK_Escape)},
          {WIN_Page_Up, XKeysymToKeycode(display, XK_Page_Up)},
          {WIN_Page_Down, XKeysymToKeycode(display, XK_Page_Down)},
          {WIN_End, XKeysymToKeycode(display, XK_End)},
          {WIN_HOME, XKeysymToKeycode(display, XK_Home)},
          {WIN_Print, XKeysymToKeycode(display, XK_Print)},
          {WIN_Insert, XKeysymToKeycode(display, XK_Insert)},
          {WIN_Delete, XKeysymToKeycode(display, XK_Delete)},
          {WIN_Pause, XKeysymToKeycode(display, XK_Pause)},
          {WIN_Scroll_Lock, XKeysymToKeycode(display, XK_Scroll_Lock)}, // seem to don't work
          {WIN_Insert, XKeysymToKeycode(display, XK_Insert)},
          {WIN_Delete, XKeysymToKeycode(display, XK_Delete)},
          {WIN_Pause, XKeysymToKeycode(display, XK_Pause)},
          {WIN_Scroll_Lock, XKeysymToKeycode(display, XK_Scroll_Lock)},
          {WIN_Num_Lock, XKeysymToKeycode(display, XK_Num_Lock)},
      }
{
}

std::optional<DecodedType> Deserializer::decode(const internal_types::Buffer& buffer) const
{
    try
    {
        return decodeInternal(buffer);
    }
    catch (...)
    {
        return std::nullopt;
    }
}

DecodedType Deserializer::decodeInternal(const internal_types::Buffer& buffer) const
{
    if (buffer[0] == serialized_values::keyEventByte)
    {
        return decodeKeyEvent(buffer);
    }
    if (buffer[0] == serialized_values::mouseMoveByte)
    {
        return decodeMouseMoveEvent(buffer);
    }
    if (buffer[0] == serialized_values::mouseScrollByte)
    {
        return decodeMouseScrollEvent(buffer);
    }
    if (buffer[0] == serialized_values::mouseKeyByte)
    {
        return decodeMouseKeyEvent(buffer);
    }
    if (buffer[0] == serialized_values::mouseChangePositionByte)
    {
        return decodeMouseChangePositionEvent(buffer);
    }
    if (buffer[0] == serialized_values::screenResolutionByte)
    {
        return decodeScreenResolution(buffer);
    }
    throw std::runtime_error("Cannot decode Event");
}

internal_types::ScreenResolution Deserializer::decodeScreenResolution(const internal_types::Buffer& buffer) const
{
    if (buffer[0] == screenResolutionByte)
    {
        return internal_types::ScreenResolution{toUInt(buffer[1], buffer[2]), toUInt(buffer[3], buffer[4])};
    }
    throw std::runtime_error("Cannot decode ScreenResolution");
}

KeyEvent Deserializer::decodeKeyEvent(const internal_types::Buffer& buffer) const
{
    return {decodeKeyCode(buffer[1]), decodeKeyState(buffer[2])};
}

KeyCode Deserializer::decodeKeyCode(const std::byte& keyId) const
{
    const auto convertedKeyId = std::to_integer<std::uint8_t>(keyId);

    if ((keyId >= WIN_A && keyId <= WIN_Z) || (keyId >= WIN_0 && keyId <= WIN_9)) // A-Z and 0-9
    {
        return XKeysymToKeycode(display, convertedKeyId);
    }
    if (keyId >= WIN_F1 && keyId <= WIN_F12) // function keys
    {
        return XKeysymToKeycode(display, convertedKeyId + WIN_LINUX_FUNCTION_KEYCODE_DIFF);
    }
    if (keyId >= WIN_BRACKETLEFT && keyId <= WIN_BRACKETRIGHT) //'[{'  ']}'  '\|'
    {
        return XKeysymToKeycode(display, convertedKeyId - WIN_LINUX_KEYCODE_DIFF_1);
    }
    if (keyId >= WIN_COMMA && keyId <= WIN_SLASH) //',<'  '-_'  '.>'  '/?'
    {
        return XKeysymToKeycode(display, convertedKeyId - WIN_LINUX_KEYCODE_DIFF_2);
    }
    if (keyId >= WIN_Left && keyId <= WIN_Down) // arrows
    {
        return XKeysymToKeycode(display, convertedKeyId + WIN_LINUX_ARROWS_KEYCODE_DIFF);
    }
    if (keyId >= WIN_NUMPAD0 && keyId <= WIN_NUMPAD9) // numpad
    {
        return XKeysymToKeycode(display, convertedKeyId + WIN_LINUX_NUMPAD_KEYCODE_DIFF);
    }
    if (keyId >= WIN_MULTIPLY && keyId <= WIN_DIVIDE) // '*'  '+'  '-'  ','  '/'
    {
        return XKeysymToKeycode(display, convertedKeyId + WIN_LINUX_KEYCODE_DIFF_3);
    }
    return translationTabel.at(keyId);
};

bool Deserializer::decodeKeyState(const std::byte& state) const
{
    if (state == serialized_values::trueValue)
    {
        return true;
    }
    if (state == serialized_values::falseValue)
    {
        return false;
    }
    throw std::runtime_error("Cannot decode KeyState");
}

MouseMoveEvent Deserializer::decodeMouseMoveEvent(const internal_types::Buffer& buffer) const
{
    return {toShort(buffer[1], buffer[2]), toShort(buffer[3], buffer[4])};
}

MouseScrollEvent Deserializer::decodeMouseScrollEvent(const internal_types::Buffer& buffer) const
{
    if (buffer.at(1) == serialized_values::scrollForwardByte)
    {
        return MouseScrollEvent::Forward;
    }
    if (buffer.at(1) == serialized_values::scrollBackwardByte)
    {
        return MouseScrollEvent::Backward;
    }
    throw std::runtime_error("Unexpected Mouse Scroll Event value");
}

MouseKeyEvent Deserializer::decodeMouseKeyEvent(const internal_types::Buffer& buffer) const
{
    switch (std::to_integer<uint8_t>(buffer.at(1)))
    {
        case leftButtonPressed:
            return MouseKeyEvent::LeftButtonPressed;
        case leftButtonUnpressed:
            return MouseKeyEvent::LeftButtonUnpressed;
        case rightButtonPressed:
            return MouseKeyEvent::RightButtonPressed;
        case rightButtonUnpressed:
            return MouseKeyEvent::RightButtonUnpressed;
        case middleButtonPressed:
            return MouseKeyEvent::MiddleButtonPressed;
        case middleButtonUnpressed:
            return MouseKeyEvent::MiddleButtonUnpressed;
    }
    throw std::runtime_error("Unexpected Mouse Key Event value");
}

MouseChangePositionEvent Deserializer::decodeMouseChangePositionEvent(const internal_types::Buffer& buffer) const
{
    return {toShort(buffer[1], buffer[2]), toShort(buffer[3], buffer[4])};
}
} // namespace internal_types
