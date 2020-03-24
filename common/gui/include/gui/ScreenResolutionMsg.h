#pragma once

#include <QMetaType>
#include "internal_types/ScreenResolution.hpp"

class ScreenResolutionMsg
{
public:
    ScreenResolutionMsg() = default;
    ~ScreenResolutionMsg() = default;
    ScreenResolutionMsg(const ScreenResolutionMsg&) = default;
    ScreenResolutionMsg& operator=(const ScreenResolutionMsg&) = default;

    ScreenResolutionMsg(const internal_types::ScreenResolution&);

    std::uint16_t width;
    std::uint16_t height;
};

Q_DECLARE_METATYPE(ScreenResolutionMsg);
