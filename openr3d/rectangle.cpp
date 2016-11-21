#include "rectangle.h"

Rectangle::Rectangle()
    : x(0.f), y(0.f), w(0.f), h(0.f) {}
Rectangle::Rectangle(float x, float y, float w, float h)
    : x(x), y(y), w(w), h(h) {}
Rectangle::Rectangle(const Rectangle& rectangle)
    : x(rectangle.x), y(rectangle.y), w(rectangle.w), h(rectangle.h) {}

void Rectangle::set(float x, float y, float w, float h)
{ this->x = x; this->y = y; this->w = w; this->h = h; }
void Rectangle::set(const Rectangle& rectangle)
{ this->x = rectangle.x; this->y = rectangle.y; this->w = rectangle.w; this->h = rectangle.h; }

Rectangle& Rectangle::operator=(const Rectangle& rectangle)
{ this->x = rectangle.x; this->y = rectangle.y; this->w = rectangle.w; this->h = rectangle.h; return *this; }
