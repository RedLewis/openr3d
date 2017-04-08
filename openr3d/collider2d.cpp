#include "collider2d.h"
#include "sceneobject.h"
#include "scene.h"
#include "physics2d.h"
#include <iostream>

Collider2D::Collider2D(SceneObject* sceneObject, ColliderType type)
    : Collider(sceneObject, type)
{
    // Creation of the body
    b2BodyDef bodyDef;
    bodyDef.position = b2Vec2(this->sceneObject->transform.getWorldPosition().x * BOX2DSCALE, this->sceneObject->transform.getWorldPosition().y * BOX2DSCALE);
    bodyDef.angle = this->sceneObject->transform.getWorldRotation().z;
    if (this->Collider::type == ColliderType::KINEMATIC)
        bodyDef.type = b2_kinematicBody;
    else if (this->Collider::type == ColliderType::STATIC)
        bodyDef.type = b2_staticBody;
    else
        bodyDef.type = b2_dynamicBody;
    body = this->sceneObject->scene->physics2d.world.CreateBody(&bodyDef);

    // Update sceneObject rigidbody boolean
    if (body->GetType() == b2_dynamicBody)
        this->sceneObject->isRigidbody = true;
}

Collider2D::~Collider2D()
{
    // Remove the body
    this->sceneObject->scene->physics2d.world.DestroyBody(this->body);

    // Update sceneObject rigidbody boolean (the "if" condition is not requiered)
    if (body->GetType() == b2_dynamicBody)
        this->sceneObject->isRigidbody = false;
}

void Collider2D::update(float deltaTime)
{
    (void)deltaTime;

    if (this->sceneObject->transform.getChangedScaleFlag())
        updateCollider();
}

void Collider2D::physicsUpdate(float deltaTime)
{
    (void)deltaTime;

    //If collider object changed position and/or rotation, update it's position and/or rotation in the physics engine
    if (this->sceneObject->transform.getChangedFlag() /*this->sceneObject->transform.changedPosition() || this->sceneObject->transform.changedRotation()*/) {
        body->SetTransform(b2Vec2(this->sceneObject->transform.getWorldPosition().x * BOX2DSCALE, this->sceneObject->transform.getWorldPosition().y * BOX2DSCALE), this->sceneObject->transform.getWorldRotation().z);
    }
    //If the physics engine changed the position of the collider, update the object's transform (for box2d it only happens on non static awake objects)
    else if (body->GetType() != b2_staticBody && body->IsAwake()) {
        Vector3 newPosition(this->sceneObject->transform.getWorldPosition());
        newPosition.x = body->GetPosition().x / BOX2DSCALE;
        newPosition.y = body->GetPosition().y / BOX2DSCALE;
        this->sceneObject->transform.setPhysicalPosition(newPosition);
        Vector3 newRotation(this->sceneObject->transform.getWorldRotation());
        newRotation.z = body->GetAngle();
        this->sceneObject->transform.setPhysicalRotation(newRotation);
    }
}
