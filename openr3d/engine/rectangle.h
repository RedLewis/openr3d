#ifndef RECTANGLE_H
#define RECTANGLE_H

class R_Rectangle
{
public:
    float x;
    float y;
    float w;
    float h;

    R_Rectangle();
    R_Rectangle(float x, float y, float w, float h);
    R_Rectangle(const R_Rectangle& rectangle);

    void set(float x, float y, float w, float h);
    void set(const R_Rectangle& rectangle);

    R_Rectangle& operator=(const R_Rectangle& rectangle);

};

#endif // RECTANGLE_H
