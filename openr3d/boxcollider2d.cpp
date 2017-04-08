#include "boxcollider2d.h"
#include "sceneobject.h"
#include "scene.h"
#include "physics2d.h"

BoxCollider2D::BoxCollider2D(SceneObject* sceneObject, ColliderType type)
    : Collider2D(sceneObject, type)
{
    // Define body fixture
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.75f;
    fixtureDef.restitution = 0.0f;
    fixtureDef.userData = this;
    updateCollider();
}

void BoxCollider2D::updateCollider()
{
    //Delete existing fixture
    if (body->GetFixtureList() != NULL)
        body->DestroyFixture(body->GetFixtureList());

    // Create new fixture
    b2PolygonShape shape;
    shape.SetAsBox((this->sceneObject->transform.getWorldScale().x / 2.f) * BOX2DSCALE, (this->sceneObject->transform.getWorldScale().y / 2.f) * BOX2DSCALE);
    fixtureDef.shape = &shape;
    body->CreateFixture(&fixtureDef);
}
