#ifndef RECTANGLE_H
#define RECTANGLE_H


class Rectangle
{
public:
    float x;
    float y;
    float w;
    float h;

    Rectangle();
    Rectangle(float x, float y, float w, float h);
    Rectangle(const Rectangle& rectangle);

    void set(float x, float y, float w, float h);
    void set(const Rectangle& rectangle);

    Rectangle& operator=(const Rectangle& rectangle);

};

#endif // RECTANGLE_H
