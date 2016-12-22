#include "color.h"

Color::Color() : r(0.0f), g(0.0f), b(0.0f), a(1.0f) {}
Color::Color(float r, float g, float b) : r(r), g(g), b(b), a(1.0f) {}
Color::Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}
Color::Color(unsigned int hex) :
    r(((float)((hex & 0xff000000) >> 24)) / (float)0xFF),
    g(((float)((hex & 0x00ff0000) >> 16)) / (float)0xFF),
    b(((float)((hex & 0x0000ff00) >> 8)) / (float)0xFF),
    a(((float)(hex & 0x000000ff)) / (float)0xFF) {}
Color::Color(const Color& color) : r(color.r), g(color.g), b(color.b), a(color.a) {}

void Color::set(float r, float g, float b) { this->r = r; this->g = g; this->b = b; }
void Color::set(float r, float g, float b, float a) { this->r = r; this->g = g; this->b = b; this->a = a; }
void Color::set(unsigned int hex) {
    this->r = ((float)((hex & 0xff000000) >> 24)) / (float)0xFF;
    this->g = ((float)((hex & 0x00ff0000) >> 16)) / (float)0xFF;
    this->b = ((float)((hex & 0x0000ff00) >> 8)) / (float)0xFF;
    this->a = ((float)(hex & 0x000000ff)) / (float)0xFF; }
void Color::set(const Color& color) { this->r = color.r; this->g = color.g; this->b = color.b; this->a = color.a; }

Color& Color::operator=(const Color& color) { this->r = color.r; this->g = color.g; this->b = color.b; this->a = color.a; return *this; }
Color& Color::operator=(unsigned int hex) {
    this->r = ((float)((hex & 0xff000000) >> 24)) / (float)0xFF;
    this->g = ((float)((hex & 0x00ff0000) >> 16)) / (float)0xFF;
    this->b = ((float)((hex & 0x0000ff00) >> 8)) / (float)0xFF;
    this->a = ((float)(hex & 0x000000ff)) / (float)0xFF;
    return *this; }

unsigned int Color::getHex() const
{
    unsigned int hex = 0;
    for (int i = 0; i < 4; ++i)
        hex += ((unsigned char)roundf(this->data[i] * (float)0xFF)) << ((3 - i) * 8);
    return hex;
}

float* Color::ptr() { return this->data; }
const float* Color::ptr() const { return this->data; }
