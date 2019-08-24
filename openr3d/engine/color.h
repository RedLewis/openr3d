#ifndef COLOR_H
#define COLOR_H

#define _USE_MATH_DEFINES
#include <cmath>

class Color
{

public:

    union {
        float data[4];
        struct {
            float r;
            float g;
            float b;
            float a;
        };
    };

    Color();
    Color(float r, float g, float b);
    Color(float r, float g, float b, float a);
    Color(unsigned int hex);
    Color(const Color& color);

    void set(float r, float g, float b);
    void set(float r, float g, float b, float a);
    void set(unsigned int hex);
    void set(const Color& color);

    Color& operator=(const Color& color);
    Color& operator=(unsigned int hex);

    unsigned int getHex() const;

    float* ptr();
    const float* ptr() const;

};

#endif // COLOR_H
