#include "color.h"

void Color::setHex(unsigned int hex)
{
    /*unsigned int mask = 0xff << 24;
    for (int i = 0; mask; ++i, mask >>= 8)
        this->data[i] = ((float)((hex & mask) >> ((3 - i) * 8))) / 0xFF;*/

    this->r = ((float)((hex & 0xff000000) >> 24)) / 0xFF;
    this->g = ((float)((hex & 0x00ff0000) >> 16)) / 0xFF;
    this->b = ((float)((hex & 0x0000ff00) >> 8)) / 0xFF;
    this->a = ((float)(hex & 0x000000ff)) / 0xFF;
}

unsigned int Color::toHex() const
{
    unsigned int hex = 0;
    for (int i = 0; i < 4; ++i)
        hex += ((unsigned char)roundf(this->data[i] * 0xFF)) << ((3 - i) * 8);
    return hex;
}
