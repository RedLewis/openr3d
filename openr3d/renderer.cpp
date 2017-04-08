#include "renderer.h"

Renderer::Renderer(SceneObject* sceneObject, Renderer::Type type)
    : Component(Component::Type::RENDERER, sceneObject), type(type)
{
}
