#ifndef COLLIDER_H
#define COLLIDER_H

#include "component.h"
#include "vector3.h"
#include <vector>

class Collider : public Component
{

public:

    struct Collision {
        SceneObject* object;
        Vector3 impulse; //impulse = force * time => impulse = mass * velocity
        Vector3 contact;
    };

    Collider(SceneObject *sceneObject);
    ~Collider();

    void update(float deltaTime);
    void draw() const;

    void onCollisionEnter(const Collision& collision);
    void onCollisionStay(const Collision& collision);
    void onCollisionExit(const Collision& collision);

};

#endif // COLLIDER_H
