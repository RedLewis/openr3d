#ifndef MESHRENDERER_H
#define MESHRENDERER_H

#include "component.h"
#include "mesh.h"
#include "texture.h"
//TODO: MATERIAL ASSETS
//#include "material.h"

class MeshRenderer : public Component
{

public:

    Mesh* mesh = NULL;
    Texture* texture = NULL;

    MeshRenderer();

    void update();
    void draw() const;
};

#endif // MESHRENDERER_H
