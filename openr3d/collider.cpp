#include "collider.h"
#include "sceneobject.h"
#include "scene.h"
#include "physics2d.h"

Collider::Collider(SceneObject* sceneObject, ColliderType type)
    : Component(Component::Type::COLLIDER, sceneObject), type(type)
{
}

Collider::~Collider()
{
}

void Collider::update(float deltaTime)
{
    (void)deltaTime;
}


void Collider::draw() const
{
}

void Collider::updateCollider()
{
}
