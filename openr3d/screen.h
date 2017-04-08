#ifndef SCREEN_H
#define SCREEN_H

namespace OpenR3D {

class Screen
{
public:
    int width;
    int height;
    float aspectRatio;

    Screen(int width, int height);
};

}

#endif // SCREEN_H
