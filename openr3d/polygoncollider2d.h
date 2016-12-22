#ifndef POLYGONCOLLIDER2D_H
#define POLYGONCOLLIDER2D_H

#include <vector>
#include "collider2d.h"
#include "polygon.h"

class PolygonCollider2D : public Collider2D
{

public:

    Polygon polygon;
    std::vector<b2Vec2> points;

    PolygonCollider2D(SceneObject *sceneObject, const std::vector<Vector3>& points, bool isStatic = false);

    void draw() const;

    void updateCollider();

};

#endif // POLYGONCOLLIDER2D_H
