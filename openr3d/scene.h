#ifndef SCENE_H
#define SCENE_H

#include <list>
#include <set>
#include "lighting.h"
#include "sceneobject.h"
#include "color.h"
#include "shaderprogram.h"
#include "matrix4.h"
#include "camera.h"
#include "light.h"
#include "asset.h"
#include "screen.h"

class Scene : public Aligned<Alignment::SSE>
{
public:

    //TODO Move screen to a openr3d static engine class (screen should not be scene dependent)
    Screen screen;
    Lighting lighting;

    //TODO: Not use pointers? But reference and actual objects in containers (easy cleanup)
    //Make object store reference to themselves in the list for fast removal.
    //List / Set / Map references stay valid when the container changes (unlike vectors)
    std::list<SceneObject*> sceneObjects;
    std::list<Asset*> assets;


    //TODO: Find a cleaner way ot handle camera and light
    std::set<Camera*> cameras;
    SceneObject* activeCamera;
    SceneObject* activeLight;

    //TODO: Use more than the standard shader
    ShaderProgram standardShader;

    Scene(int width, int height);
    ~Scene();

    void draw() const;
    void update(float deltaTime);

};

#endif // SCENE_H
