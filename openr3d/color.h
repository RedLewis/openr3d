#ifndef COLOR_H
#define COLOR_H

#include "vector4.h"

class Color : public Vector4 {

public:

    float& r = Vector4::x;
    float& g = Vector4::y;
    float& b = Vector4::z;
    float& a = Vector4::w = 1.0f;

    using Vector4::Vector4;

    void setHex(unsigned int hex);
    unsigned int toHex() const;

};

#endif // COLOR_H
