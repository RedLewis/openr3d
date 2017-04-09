#ifndef SPRITERENDERER_H
#define SPRITERENDERER_H

#include "renderer.h"
#include "polygon.h"
#include "texture.h"

class SpriteRenderer : public Renderer {

public:

    Polygon* polygon = NULL;
    Texture* texture = NULL;

    SpriteRenderer(SceneObject* sceneObject);

    void update(float deltaTime);
    void draw() const;

};

#endif // SPRITERENDERER_H
