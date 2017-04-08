#include "color32.h"

Color32::Color32() : r(0), g(0), b(0), a(255) {}
Color32::Color32(unsigned char r, unsigned char g, unsigned char b) : r(r), g(g), b(b), a(255) {}
Color32::Color32(unsigned char r, unsigned char g, unsigned char b, unsigned char a) : r(r), g(g), b(b), a(a) {}
Color32::Color32(unsigned int hex) :
    r((hex & 0xff000000) >> 24),
    g((hex & 0x00ff0000) >> 16),
    b((hex & 0x0000ff00) >> 8),
    a(hex & 0x000000ff) {}
Color32::Color32(const Color32& other) : ui32(other.ui32) {}

void Color32::set(unsigned char r, unsigned char g, unsigned char b) { this->r = r; this->g = g; this->b = b; }
void Color32::set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { this->r = r; this->g = g; this->b = b; this->a = a; }
void Color32::set(unsigned int hex) {
    this->r = (hex & 0xff000000) >> 24;
    this->g = (hex & 0x00ff0000) >> 16;
    this->b = (hex & 0x0000ff00) >> 8;
    this->a = (hex & 0x000000ff); }
void Color32::set(const Color32& color) { this->ui32 = color.ui32; }

Color32& Color32::operator=(const Color32& other) { this->ui32 = other.ui32; }
Color32& Color32::operator=(unsigned int hex) {
    this->r = (hex & 0xff000000) >> 24;
    this->g = (hex & 0x00ff0000) >> 16;
    this->b = (hex & 0x0000ff00) >> 8;
    this->a = (hex & 0x000000ff);
    return *this; }

unsigned int Color32::getHex() const {
    unsigned int hex = 0;
    hex += this->data[0] << 24;
    hex += this->data[1] << 16;
    hex += this->data[2] << 8;
    hex += this->data[3];
    return hex;
}

unsigned char* Color32::ptr() { return this->data; }
const unsigned char* Color32::ptr() const { return this->data; }
