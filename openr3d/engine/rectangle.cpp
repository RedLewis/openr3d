#include "rectangle.h"

R_Rectangle::R_Rectangle()
    : x(0.f), y(0.f), w(0.f), h(0.f) {}
R_Rectangle::R_Rectangle(float x, float y, float w, float h)
    : x(x), y(y), w(w), h(h) {}
R_Rectangle::R_Rectangle(const R_Rectangle& rectangle)
    : x(rectangle.x), y(rectangle.y), w(rectangle.w), h(rectangle.h) {}

void R_Rectangle::set(float x, float y, float w, float h)
{ this->x = x; this->y = y; this->w = w; this->h = h; }
void R_Rectangle::set(const R_Rectangle& rectangle)
{ this->x = rectangle.x; this->y = rectangle.y; this->w = rectangle.w; this->h = rectangle.h; }

R_Rectangle& R_Rectangle::operator=(const R_Rectangle& rectangle)
{ this->x = rectangle.x; this->y = rectangle.y; this->w = rectangle.w; this->h = rectangle.h; return *this; }

