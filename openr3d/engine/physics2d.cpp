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
    if (colliderA == NULL || colliderB == NULL
            || (colliderA->onCollisionEnterPtr == NULL && colliderB->onCollisionEnterPtr == NULL))
        return;

    Collision collision;
    b2WorldManifold worldManifold;
    contact->GetWorldManifold(&worldManifold);
    collision.impulse.set(worldManifold.normal.x, worldManifold.normal.y, 0.0f);
    collision.contact.set(worldManifold.points[0].x / BOX2DSCALE, worldManifold.points[0].y / BOX2DSCALE, 0.0f);

    if (colliderA->onCollisionEnterPtr != NULL) {
        collision.collider = colliderB;
        colliderA->onCollisionEnterPtr(colliderA, collision);
    }

    if (colliderB->onCollisionEnterPtr != NULL) {
        collision.collider = colliderA;
        colliderB->onCollisionEnterPtr(colliderB, collision);
    }
}


//TODO: Invalid read, exit point and exit direction are not set!
void Physics2D::ContactListener::EndContact(b2Contact* contact)
{
    Collider* colliderA = static_cast<Collider*>(contact->GetFixtureA()->GetUserData());
    Collider* colliderB = static_cast<Collider*>(contact->GetFixtureB()->GetUserData());
    if (colliderA == NULL || colliderB == NULL
            || (colliderA->onCollisionExitPtr == NULL && colliderB->onCollisionExitPtr == NULL))
        return;

    Collision collision;
    b2WorldManifold worldManifold;
    contact->GetWorldManifold(&worldManifold);
    collision.impulse.set(worldManifold.normal.x, worldManifold.normal.y, 0.0f);
    collision.contact.set(worldManifold.points[0].x / BOX2DSCALE, worldManifold.points[0].y / BOX2DSCALE, 0.0f);

    if (colliderA->onCollisionExitPtr != NULL) {
        collision.collider = colliderB;
        colliderA->onCollisionExitPtr(colliderA, collision);
    }

    if (colliderB->onCollisionExitPtr != NULL) {
        collision.collider = colliderA;
        colliderB->onCollisionExitPtr(colliderB, collision);
    }

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
