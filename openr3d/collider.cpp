#include "collider.h"

Collider::Collider(SceneObject* sceneObject)
    : Component(Component::Type::COLLIDER, sceneObject)
{
}

Collider::~Collider()
{
}

void Collider::update(float deltaTime)
{
}


void Collider::draw() const
{
}
