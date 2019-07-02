#include <X11/Xlib.h>

class FakeKey
{
public:
    FakeKey();
    ~FakeKey();
    void pressKey(u_int16_t keyId);

private:
    Display* display;
};
