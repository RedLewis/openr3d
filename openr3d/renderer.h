#ifndef RENDERER_H
#define RENDERER_H

#include "component.h"

class Renderer : public Component {

public:

    enum Type {
        MESH_RENDERER,
        SPRITE_RENDERER
    };

    const Renderer::Type type;

    Renderer(SceneObject* sceneObject, Renderer::Type type);

};

#endif // RENDERER_H
