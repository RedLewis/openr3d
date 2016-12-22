#include "collider2d.h"
#include "sceneobject.h"
#include "scene.h"
#include "physics2d.h"
#include <iostream>

Collider2D::Collider2D(SceneObject* sceneObject, bool isStatic)
    : Collider(sceneObject)
{
    // Creation of the body
    b2BodyDef bodyDef;
    bodyDef.position = b2Vec2(this->sceneObject->transform.getWorldPosition().x * BOX2DSCALE, this->sceneObject->transform.getWorldPosition().y * BOX2DSCALE);
    bodyDef.angle = this->sceneObject->transform.getWorldRotation().z;
    bodyDef.type = isStatic ? b2_staticBody : b2_dynamicBody;
    body = this->sceneObject->scene->physics2d.world.CreateBody(&bodyDef);

    // Update sceneObject rigidbody boolean
    if (body->GetType() == b2_dynamicBody)
        this->sceneObject->isRigidbody = true;
}

Collider2D::~Collider2D()
{
    // Remove the body
    this->sceneObject->scene->physics2d.world.DestroyBody(this->body);

    // Update sceneObject rigidbody boolean (IF STATEMENT NOT REQUIERED
    if (body->GetType() == b2_dynamicBody)
        this->sceneObject->isRigidbody = false;
}

void Collider2D::update(float deltaTime)
{
    (void)deltaTime;

    if (this->sceneObject->transform.changedScale())
        updateCollider();

}

void Collider2D::physicsUpdate(float deltaTime)
{
    (void)deltaTime;

    if (this->sceneObject->transform.changedPosition() || this->sceneObject->transform.changedRotation()) {
        body->SetTransform(b2Vec2(this->sceneObject->transform.getWorldPosition().x * BOX2DSCALE, this->sceneObject->transform.getWorldPosition().y * BOX2DSCALE), this->sceneObject->transform.getWorldRotation().z);
    }
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
