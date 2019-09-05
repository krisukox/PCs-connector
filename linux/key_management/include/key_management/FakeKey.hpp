#pragma once

#include <X11/Xlib.h>
#include "key_management/IKey.hpp"

namespace key_management
{
class FakeKey : public IKey
{
public:
    FakeKey(Display*);
    ~FakeKey() override;

    void onEvent(const internal_types::KeyEvent& keyEvent) const override;

private:
    Display* display;
};
} // namespace key_management
