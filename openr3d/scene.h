#ifndef SCENE_H
#define SCENE_H

#include <list>
#include "lighting.h"
#include "sceneobject.h"
#include "color.h"

class Scene : public Aligned<16>
{
public:

    Lighting lighting;
    std::list<SceneObject*> sceneObjects;

    Scene();

    void draw() const;
    void update();

};

#endif // SCENE_H
