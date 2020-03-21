#pragma once
#include <QWidget>
#include "gui/ScreenResolutionMsg.h"

class MsgSender : public QWidget
{
    Q_OBJECT

public:
    MsgSender();

    void send(const ScreenResolutionMsg&);
signals:
    void messageSent(const ScreenResolutionMsg&);
};
