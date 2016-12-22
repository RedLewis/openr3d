#include "physics2d.h"
#include "collider.h"
#include "scene.h"

Physics2D::ContactListener::ContactListener()
{
}


void Physics2D::ContactListener::BeginContact(b2Contact* contact)
{
    Collider* colliderA = static_cast<Collider*>(contact->GetFixtureA()->GetUserData());
    Collider* colliderB = static_cast<Collider*>(contact->GetFixtureB()->GetUserData());
    if (colliderA == NULL || colliderB == NULL)
        return;

    Collider::Collision collision;
    b2WorldManifold worldManifold;
    contact->GetWorldManifold(&worldManifold);
    collision.impulse.set(worldManifold.normal.x, worldManifold.normal.y, 0.0f);
    collision.contact.set(worldManifold.points[0].x / BOX2DSCALE, worldManifold.points[0].y / BOX2DSCALE, 0.0f);

    collision.collider = colliderB;
    colliderA->onCollisionEnter(collision);

    collision.collider = colliderA;
    colliderB->onCollisionEnter(collision);
}

void Physics2D::ContactListener::EndContact(b2Contact* contact)
{
    Collider* colliderA = static_cast<Collider*>(contact->GetFixtureA()->GetUserData());
    Collider* colliderB = static_cast<Collider*>(contact->GetFixtureB()->GetUserData());
    if (colliderA == NULL || colliderB == NULL)
        return;

    Collider::Collision collision;
    b2WorldManifold worldManifold;
    contact->GetWorldManifold(&worldManifold);
    collision.impulse.set(worldManifold.normal.x, worldManifold.normal.y, 0.0f);
    collision.contact.set(worldManifold.points[0].x / BOX2DSCALE, worldManifold.points[0].y / BOX2DSCALE, 0.0f);

    collision.collider = colliderB;
    colliderA->onCollisionExit(collision);

    collision.collider = colliderA;
    colliderB->onCollisionExit(collision);
}

Physics2D::Physics2D()
    : world(b2Vec2(0.f, -9.8f))
{
    world.SetContactListener(&contactListener);
}

void Physics2D::update(float deltaTime)
{
    world.Step(deltaTime, 8, 3);
}
