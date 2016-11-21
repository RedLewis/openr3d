#include "meshrenderer.h"
#include "opengl.h"
#include "shaderprogram.h"

MeshRenderer::MeshRenderer(SceneObject* sceneObject)
    : Component(Component::Type::MESHRENDERER, sceneObject)
{

}

void MeshRenderer::update(float deltaTime)
{

}


void MeshRenderer::draw() const
{
    if (texture != NULL)
        texture->draw();

    if (mesh != NULL)
        mesh->draw();

    // TODO: Find cleaner way of unbinding texture (it must be done after drawing)
    // TODO: Maybe move the texture draw and mesh draw to the mesh renderer draw
    // (After all it's the renderer's job to render ain't it?)
    if (texture != NULL) {
        gl->glBindTexture(GL_TEXTURE_2D, 0);
    }
}
