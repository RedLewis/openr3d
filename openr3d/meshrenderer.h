#ifndef MESHRENDERER_H
#define MESHRENDERER_H

#include "renderer.h"
#include "mesh.h"
#include "texture.h"
//TODO: MATERIAL ASSETS
//#include "material.h"

class MeshRenderer : public Renderer
{

public:

    Mesh* mesh = NULL;
    Texture* texture = NULL;

    MeshRenderer(SceneObject* sceneObject);

    void update(float deltaTime);
    void draw() const;
};

#endif // MESHRENDERER_H
