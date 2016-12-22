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
    Color color;

    Light(SceneObject* sceneObject, Light::Type type);

    void update(float deltaTime);
    void draw() const;

};

#endif // LIGHT_H
