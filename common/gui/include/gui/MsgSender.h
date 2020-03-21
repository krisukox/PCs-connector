#pragma once
#include <QWidget>
#include "gui/ScreenResolutionMsg.h"

class MsgSender : public QWidget
{
    Q_OBJECT

public:
    MsgSender();

    //    send();

signals:
    void messageSent(const ScreenResolutionMsg&);
};
