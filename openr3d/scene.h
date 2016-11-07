#ifndef SCENE_H
#define SCENE_H

#include <list>
#include "lighting.h"
#include "sceneobject.h"
#include "color.h"
#include "shaderprogram.h"
#include "matrix4.h"
#include "camera.h"
#include "light.h"

class Scene : public Aligned<16>
{
public:

    Lighting lighting;
    std::list<SceneObject*> sceneObjects;


    //TODO: Find a cleaner way ot handle camera and light
    Camera* activeCamera;
    Light* activeLight;
    //TODO: Use more than the standard shader
    ShaderProgram standardShader;

    Scene();

    void draw() const;
    void update();

};

#endif // SCENE_H