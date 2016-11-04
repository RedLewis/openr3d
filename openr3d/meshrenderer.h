#ifndef MESHRENDERER_H
#define MESHRENDERER_H

#include "component.h"
#include "mesh.h"
//TODO: TEXUTRE AND MATERIAL ASSETS
//#include "texture.h"
//#include "material.h"

class MeshRenderer : public Component
{

public:

    Mesh* mesh;


    MeshRenderer();

    void update();
    void draw() const;
};

#endif // MESHRENDERER_H
