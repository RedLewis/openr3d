#ifndef LIGHT_H
#define LIGHT_H

#include "component.h"
#include "color.h"
#include "opengl.h"

class Light : public Component
{

public:

    enum Type {
        DIRECTIONAL,
        POINT,
        SPOT
    };

    Light::Type type;
    Color ambientColor;
    Color diffuseColor;
    Color specularColor;


    unsigned int bindID;
    static unsigned int nextBindID;

    Light(Light::Type type);

    void update();
    void draw() const;

};

#endif // LIGHT_H
