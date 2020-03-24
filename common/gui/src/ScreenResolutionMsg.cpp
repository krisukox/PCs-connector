#include "gui/ScreenResolutionMsg.h"

ScreenResolutionMsg::ScreenResolutionMsg(const internal_types::ScreenResolution& screenResolution)
    : width{screenResolution.width}, height{screenResolution.height}
{
}
