#ifndef CIRCLECOLLIDER2D_H
#define CIRCLECOLLIDER2D_H

#include "collider2d.h"

class CircleCollider2D : public Collider2D
{
public:

    CircleCollider2D(SceneObject *sceneObject, bool isStatic = false);

    void updateCollider();

};

#endif // CIRCLECOLLIDER2D_H
