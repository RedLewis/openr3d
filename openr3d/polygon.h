#ifndef POLYGON_H
#define POLYGON_H

#include "asset.h"
#include "vector3.h"
#include <vector>
#include <list>
#include <Box2D/Box2D.h>

class Polygon : public Asset
{

public:

    struct ConvexFace {
        std::vector<b2Vec2> points;
    };

    std::vector<b2Vec2> points;
    std::list<ConvexFace> convexFaces;

    Polygon();
    Polygon(const std::vector<Vector3>& points);
    //TODO: Handle the destruction case when no date have has been loaded
    ~Polygon();

    int load(const std::vector<Vector3>& points);

    void draw() const;
};

#endif // POLYGON_H
