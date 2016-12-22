#include "circlecollider2d.h"
#include "sceneobject.h"
#include "scene.h"
#include "physics2d.h"

CircleCollider2D::CircleCollider2D(SceneObject *sceneObject, bool isStatic)
    : Collider2D(sceneObject, isStatic)
{
    // Define body fixture
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.75f;
    fixtureDef.restitution = 0.5f;
    fixtureDef.userData = this;
    updateCollider();
}

void CircleCollider2D::updateCollider()
{
    //Delete existing fixture
    if (body->GetFixtureList() != NULL)
        body->DestroyFixture(body->GetFixtureList());

    // Create new fixture
    b2CircleShape shape;
    if (fabs(this->sceneObject->transform.getWorldScale().x) > fabs(this->sceneObject->transform.getWorldScale().y))
        shape.m_radius = (this->sceneObject->transform.getWorldScale().x / 2.f) * BOX2DSCALE;
    else
        shape.m_radius = (this->sceneObject->transform.getWorldScale().y / 2.f) * BOX2DSCALE;
    fixtureDef.shape = &shape;
    body->CreateFixture(&fixtureDef);
}
