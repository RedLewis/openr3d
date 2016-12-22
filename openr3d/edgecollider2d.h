#ifndef EDGECOLLIDER2D_H
#define EDGECOLLIDER2D_H

#include <vector>
#include "collider2d.h"

class EdgeCollider2D : public Collider2D
{
public:

    std::vector<b2Vec2> points;

    EdgeCollider2D(SceneObject *sceneObject, const std::vector<Vector3>& points, bool isStatic = false);

    void updateCollider();

};

#endif // EDGECOLLIDER2D_H
