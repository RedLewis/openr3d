#ifndef COLLIDER2D_H
#define COLLIDER2D_H

#include "collider.h"
#include <Box2D/Box2D.h>

class Collider2D : public Collider
{
public:

    std::list<Collider2D*>::iterator containerIterator;

    b2Body* body;
    b2FixtureDef fixtureDef;

    virtual void update(float deltaTime);
    virtual void physicsUpdate(float deltaTime);

    virtual void updateCollider() = 0;

    virtual ~Collider2D();
protected:
    Collider2D(SceneObject *sceneObject, bool isStatic = false);
};

#endif // COLLIDER2D_H
