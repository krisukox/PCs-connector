#pragma once
#include <QMetaType>

class ScreenResolutionMsg
{
public:
    QString body() const;
};

Q_DECLARE_METATYPE(ScreenResolutionMsg)
