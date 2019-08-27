#include "meshrenderer.h"
#include "opengl.h"
#include "shaderprogram.h"

MeshRenderer::MeshRenderer(SceneObject* sceneObject)
    : Renderer(sceneObject, Renderer::Type::MESH_RENDERER)
{

}

void MeshRenderer::update(float deltaTime)
{
    (void)deltaTime;
}


void MeshRenderer::draw() const
{
    if (texture != NULL)
        texture->draw();

    if (normalMap != NULL)
        normalMap->draw();

    if (mesh != NULL)
        mesh->draw();

    // TODO: Find cleaner way of unbinding textures (it must be done after drawing)
    // TODO: Maybe move the texture draw and mesh draw to the mesh renderer draw
    // (After all it's the renderer's job to render ain't it?)
    //Unbind texutre
    if (texture != NULL) {
        gl->glActiveTexture(GL_TEXTURE0);
        gl->glBindTexture(GL_TEXTURE_2D, 0);
        //Make sure uniform useTexture is set back to 0
        gl->glUniform1i(ShaderProgram::activeShaderProgram->useTextureIndex, 0);
    }
    //Unbind normal map
    if (normalMap != NULL) {
        gl->glActiveTexture(GL_TEXTURE1);
        gl->glBindTexture(GL_TEXTURE_2D, 0);
        //Make sure uniform useNormalMap is set back to 0
        gl->glUniform1i(ShaderProgram::activeShaderProgram->useNormalMapIndex, 0);
    }
    //Reset active texture unit to initial value of GL_TEXTURE0
    gl->glActiveTexture(GL_TEXTURE0);
}
