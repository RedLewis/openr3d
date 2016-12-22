#ifndef COLLIDER_H
#define COLLIDER_H

#include "component.h"
#include "vector3.h"
#include <list>

class Collider : public Component
{

public:

    struct Collision {
        Collider* collider; // The other collider that is hit
        Vector3 contact; // The point of contact
        Vector3 impulse; //impulse = force * time => impulse = mass * velocity
    };

    virtual void update(float deltaTime) = 0;
    virtual void physicsUpdate(float deltaTime) = 0;
    virtual void draw() const;

    virtual void updateCollider() = 0;

    virtual void onCollisionEnter(const Collision& collision);
    virtual void onCollisionStay(const Collision& collision);
    virtual void onCollisionExit(const Collision& collision);

    virtual ~Collider();
protected:
    Collider(SceneObject *sceneObject);

};

#endif // COLLIDER_H
