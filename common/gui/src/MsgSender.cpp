#include "gui/MsgSender.h"
#include <QMetaType>

MsgSender::MsgSender()
{
    qRegisterMetaType<ScreenResolutionMsg>("ScreenResolutionMsg");
}

void MsgSender::send(const ScreenResolutionMsg& screenResolutionMsg)
{
    //    ScreenResolutionMsg msg;
    //    msg.width = 1920;
    //    msg.height = 1080;
    emit messageSent(screenResolutionMsg);
}
