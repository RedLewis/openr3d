#ifndef COLLIDER_H
#define COLLIDER_H

#include "component.h"
#include "vector3.h"
#include <list>

struct Collision;

class Collider : public Component
{

public:

    enum ColliderType {
        DYNAMIC,
        STATIC,
        KINEMATIC
    };

    virtual void update(float deltaTime) = 0;
    virtual void physicsUpdate(float deltaTime) = 0;
    virtual void draw() const;

    virtual void updateCollider() = 0;

    void (*onCollisionEnterPtr)(Collider* thisCollider, const Collision& collision) = NULL;
    void (*onCollisionStayPtr)(Collider* thisCollider, const Collision& collision) = NULL;
    void (*onCollisionExitPtr)(Collider* thisCollider, const Collision& collision) = NULL;

    virtual ~Collider();
protected:

    ColliderType type;
    Collider(SceneObject *sceneObject, ColliderType type);

};

//TODO In physics 2D set full instead fo just the normal of impulse
struct Collision {
    Collider* collider; // The other collider that is hit
    Vector3 contact; // The point of contact
    Vector3 impulse; //impulse = force * time => impulse = mass * velocity
};

#endif // COLLIDER_H
