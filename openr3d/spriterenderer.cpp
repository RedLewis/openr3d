#include "spriterenderer.h"
#include "opengl.h"
#include "shaderprogram.h"

SpriteRenderer::SpriteRenderer(SceneObject* sceneObject)
    : Renderer(sceneObject, Renderer::Type::SPRITE_RENDERER)
{

}

void SpriteRenderer::update(float deltaTime)
{
    (void)deltaTime;
}


void SpriteRenderer::draw() const
{
    if (texture != NULL)
        texture->draw();

    if (polygon != NULL)
        polygon->draw();

    // TODO: Find cleaner way of unbinding texture (it must be done after drawing)
    // TODO: Maybe move the texture draw and sprite draw to the sprite renderer draw
    // (After all it's the renderer's job to render ain't it?)
    if (texture != NULL) {
        gl->glBindTexture(GL_TEXTURE_2D, 0);
    }

    //Make sure useTextureIndex is set back to 0
    gl->glUniform1i(ShaderProgram::activeShaderProgram->useTextureIndex, 0);
}
