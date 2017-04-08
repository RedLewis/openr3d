#ifndef COLOR32_H
#define COLOR32_H

#define _USE_MATH_DEFINES
#include <math.h>

class Color32
{

public:

    union {
        unsigned int ui32;
        unsigned char data[4];
        struct {
            unsigned char r;
            unsigned char g;
            unsigned char b;
            unsigned char a;
        };
    };

    Color32();
    Color32(unsigned char r, unsigned char g, unsigned char b);
    Color32(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
    Color32(unsigned int hex);
    Color32(const Color32& color);

    void set(unsigned char r, unsigned char g, unsigned char b);
    void set(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
    void set(unsigned int hex);
    void set(const Color32& color);

    Color32& operator=(const Color32& color);
    Color32& operator=(unsigned int hex);

    unsigned int getHex() const;

    unsigned char* ptr();
    const unsigned char* ptr() const;

};

#endif // Color32_H
