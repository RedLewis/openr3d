#include "collider.h"
#include "sceneobject.h"
#include "scene.h"
#include "physics2d.h"

Collider::Collider(SceneObject* sceneObject)
    : Component(Component::Type::COLLIDER, sceneObject)
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

void Collider::onCollisionEnter(const Collision& collision)
{
    (void)collision;
}

void Collider::onCollisionStay(const Collision& collision)
{
    (void)collision;
}

void Collider::onCollisionExit(const Collision& collision)
{
    (void)collision;
}
