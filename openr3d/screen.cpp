#include "screen.h"

OpenR3D::Screen::Screen(int width, int height)
    : width(width), height(height)
{
    aspectRatio = (float)width / (float)height;
}
