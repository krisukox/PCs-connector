#pragma once
#include <QMetaType>

class ScreenResolutionMsg
{
public:
    ScreenResolutionMsg() = default;
    ~ScreenResolutionMsg() = default;
    ScreenResolutionMsg(const ScreenResolutionMsg&) = default;
    ScreenResolutionMsg& operator=(const ScreenResolutionMsg&) = default;

    std::uint16_t width;
    std::uint16_t height;
};

Q_DECLARE_METATYPE(ScreenResolutionMsg);
