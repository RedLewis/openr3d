#include "meshrenderer.h"
#include "opengl.h"

MeshRenderer::MeshRenderer()
    : Component(Component::Type::MESHRENDERER)
{

}

void MeshRenderer::update()
{

}


void MeshRenderer::draw() const
{
    if (texture != NULL) {
        texture->draw();
    }
    if (mesh != NULL) {
        mesh->draw();
    }
}
